import express from "express";
import fs from "fs";
import path from "path";
import { fileURLToPath } from "url";

const app = express();

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
app.use(express.static(path.join(__dirname, "build")));

// use body parser to get JSON object from client
app.use(express.json());
app.use(express.text());

// new recipe request
app.get("/new_recipe", (req, res) => {
    res.sendFile(
        path.resolve(__dirname, "build", "index.html")
    );
});

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
    // get recipe's name
    let fetchedRecipeName = req.body;

    // get recipes.json file to modify
    let recipes = JSON.parse(fs.readFileSync("recipes.json"));
    delete recipes[fetchedRecipeName];

    // update JSON file
    let rawData = JSON.stringify(recipes, null, 4);
    fs.writeFileSync("recipes.json", rawData);
});

app.listen(5000);
