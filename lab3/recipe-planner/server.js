import express from "express";
import fs from "fs";
import path from "path";

const app = express();

// use body parser to get JSON object from client
app.use(express.json());
app.use(express.text());

// get file with recipes
app.get("/recipes.json", (_req, res) => {
    let rawData = fs.readFileSync("recipes.json");
    res.json(JSON.parse(rawData));
});

// add new recipe to the file
app.post("/add", (req, _res) => {
    // get recipe's content
    let fetchedRecipe = req.body;

    // get recipes.json file to modify
    let recipes = JSON.parse(fs.readFileSync("recipes.json"));
    let recipeName = Object.keys(fetchedRecipe)[0];
    recipes[recipeName] = fetchedRecipe[recipeName];

    // update JSON file
    let rawData = JSON.stringify(recipes, null, 4);
    fs.writeFileSync("recipes.json", rawData);
});

// delete recipe
app.post("/delete", (req, _res) => {
    // get recipe's content
    let fetchedRecipeName = req.body;

    // get recipes.json file to modify
    let recipes = JSON.parse(fs.readFileSync("recipes.json"));
    delete recipes[fetchedRecipeName];

    // update JSON file
    let rawData = JSON.stringify(recipes, null, 4);
    fs.writeFileSync("recipes.json", rawData);
})

app.listen(5000);
