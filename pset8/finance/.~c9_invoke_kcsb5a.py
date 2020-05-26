import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT symbol, name,  SUM(shares) AS 'shares', ROUND(SUM(shares * price) / SUM(shares),2) AS 'price', ROUND(SUM(shares * price),2) AS 'total' FROM transactions WHERE user_id = :id GROUP BY symbol",
                        id=session["user_id"])
                        
    cash = db.execute("SELECT ROUND(cash,2) AS 'cash' FROM users WHERE id =:id", 
                      id=session["user_id"])
    total = 0
    
    for stock in stocks:
        if stock['shares'] != 0:
            total = total + stock['total']
    total = usd(total + cash[0]['cash'])
    
    return render_template("index.html", stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        stock = lookup(request.form.get("symbol").strip())
        shares = int(request.form.get("shares").strip())
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        
        if not stock:
            return apology("Please enter a valid symbol", 400)
        # below if statement will never happen. Form will not allow submission for values < 1
        elif shares < 1:
            return apology("Enter valid number of shares to purchase", 400)
        elif stock["price"] * shares > cash[0]["cash"]:
            return apology("get more money")
        else:
            # stores transaction
            db.execute("INSERT INTO transactions (user_id, symbol, name, shares, price) VALUES (?,?,?,?,?)", 
                       (session["user_id"]), stock["symbol"], stock["name"], shares, stock["price"])
                        
            # Updates current cash amount... is this the best way?
            db.execute("UPDATE users SET cash = :new_cash  WHERE id = :id", 
                       new_cash=cash[0]["cash"]-(shares*stock["price"]), id=session["user_id"])
            flash("Stock Purchased!")
            return redirect("/")
    else:
        return render_template("buy.html")
    

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, price, transacted FROM transactions WHERE user_id = :id",
                              id=session["user_id"])

    return render_template("history.html", transactions=transactions)
    

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Welcome to CS50 Finance")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    flash("Successfully Logged Out")
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # lookup stock ticker, with .strip to remove whitespace
        stock = lookup(request.form.get("symbol").strip())
        if not stock:
            return apology("Please enter a valid symbol", 400)
        else:
            return render_template("quoted.html", name=stock["name"], price=stock["price"], symbol=stock["symbol"])
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        in_user = request.form.get("username")
        in_pass = request.form.get("password")
        
        # Ensure username was submitted
        if not in_user:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif (not in_pass or in_pass != request.form.get("confirmation")):
            return apology("must provide password, passwords must match", 403)
        
        elif db.execute("SELECT username FROM users WHERE username = :username", username=in_user):
            return apology("Username is taken", 403)

        # Insert valid entry into database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                   username=in_user, hash=generate_password_hash(in_pass, method='pbkdf2:sha256', salt_length=8))
    
        # # Remember which user has logged in
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = :username", username=in_user)
        
        # Redirect user to home page
        flash("Successfully Registered, Welcome to CS50 Finance")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocks = db.execute("SELECT symbol, SUM(shares) AS 'shares' FROM transactions WHERE user_id = :id GROUP BY symbol",
                        id=session["user_id"])
                        
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Select Symbol", 400)

        stock = lookup(symbol)
        shares = int(request.form.get("shares").strip())
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        
        for row in stocks:
            if row['symbol'] == symbol:
                if shares > row['shares']:
                    return apology("Selling more than you have?", 400)
                else:
                    # stores transaction
                    db.execute("INSERT INTO transactions (user_id, symbol, name, shares, price) VALUES (?,?,?,?,?)", 
                               (session["user_id"]), stock["symbol"], stock["name"], -shares, stock["price"])
                                
                    # Updates current cash amount... is this the best way?
                    db.execute("UPDATE users SET cash = :new_cash  WHERE id = :id", 
                               new_cash=cash[0]["cash"]+(shares*stock["price"]), id=session["user_id"])
                    
                    flash("Shares Successfully Sold!")           
                    return redirect("/")
                
    else:
        return render_template("sell.html", stocks=stocks)


# Personal Touch, change password
@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change():
    if request.method == "POST":
        current_pass = request.form.get("current_password")
        new_pass = request.form.get("new_password")
        
        if not current_pass or not new_pass or new_pass != request.form.get("confirmation"):
            return apology("must provide password, passwords must match", 403)
        
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                          id=session["user_id"])
                          
        if not check_password_hash(rows[0]['hash'], current_pass):
            return apology("Current Password Incorrect", 400)

        # Update password hash in database
        db.execute("UPDATE users SET hash = :new_hash  WHERE id = :id",
                   id=session['user_id'], new_hash=generate_password_hash(new_pass, method='pbkdf2:sha256', salt_length=8))
                    
        flash("Your Password has been Changed")
        return redirect("/")
        
    else:
        return render_template("change_password.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)