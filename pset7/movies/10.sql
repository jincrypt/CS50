/*
In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
Your query should output a table with a single column for the name of each person.
1841
*/

SELECT DISTINCT people.name from movies
JOIN directors ON directors.movie_id = movies.id
JOIN people ON people.id = directors.person_id
JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0
