import express from "express";
import fs from "fs";
import path from "path";

const app = express();

app.get("/recipes.json", (_req, res) => {
    // get file with recipes
    let rawData = fs.readFileSync("recipes.json");
    res.json(JSON.parse(rawData));
});

app.listen(5000);
