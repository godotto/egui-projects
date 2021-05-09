﻿using System;
using System.Collections.Generic;
using System.Linq;

using System.Text.Json;
using System.Text.Json.Serialization;

using Microsoft.AspNetCore.Mvc;
using recipe_planner.Models;

namespace recipe_planner.Controllers
{
    public class HomeController : Controller
    {    
        // list of recipe models
        static List<RecipeModel> recipes = new List<RecipeModel>();

        public HomeController()
        {
            // if list is empty, read recipes from JSON file
            if (recipes.Count == 0)
                ReadRecipeFile();
        }

        public IActionResult Index()
        {
            return View(recipes);
        }

        public IActionResult NewRecipe()
        {
            return View(new RecipeModel());
        }

        [HttpPost]
        public IActionResult AddRecipe(string Name, string Description)
        {
            // create new recipe model object and assign recipe's name
            var newRecipe = new RecipeModel();
            newRecipe.Name = Name;

            // remove \r characters from line breaks
            Description = Description.Replace('\r', '\0');

            // save splited description to new recipe object
            newRecipe.Description = Description.Split('\n').ToList();

            // add recipe to the list and return to the main view
            recipes.Add(newRecipe);
            return RedirectToAction("Index");
        }

        private void ReadRecipeFile()
        {
            // read JSON file and deserialize it
            var rawJson = System.IO.File.ReadAllText("recipes.json");
            var deserializedJson = JsonSerializer.Deserialize<JsonElement>(rawJson);

            // fill list of models in loop
            foreach (var recipeJson in deserializedJson.EnumerateObject())
            {
                // create new recipe object and get the name of recipe
                var newRecipe = new RecipeModel();
                newRecipe.Name = recipeJson.Name;

                // define lists with description and ingredients
                newRecipe.Description = new List<string>();
                newRecipe.Ingredients = new List<IngredientModel>();

                // get content of recipe JSON object and get the rest information from JSON
                var recipeContent = recipeJson.Value;
                foreach (var recipeContentElement in recipeContent.EnumerateObject())
                {
                    // if name of property is recipe, then it is recipe's descritpion
                    if (recipeContentElement.NameEquals("recipe"))
                    {
                        // add description to new recipe object
                        foreach (var descriptionLine in recipeContentElement.Value.EnumerateArray())
                            newRecipe.Description.Add(descriptionLine.GetString());
                    }
                    else
                    {
                        // create new ingredient object and get its name, quantity and unit
                        var newIngredient = new IngredientModel();
                        newIngredient.Name = recipeContentElement.Name;
                        newIngredient.Quantity = CutQuantity(recipeContentElement.Value.GetString());
                        newIngredient.Unit = CutUnit(recipeContentElement.Value.GetString());

                        // add ingredient to the list
                        newRecipe.Ingredients.Add(newIngredient);
                    }
                }

                // add recipe to the list
                recipes.Add(newRecipe);
            }
        }

        // get quantity from string
        private float CutQuantity(string qunatityWithUnit)
        {
            return Convert.ToSingle(qunatityWithUnit.Substring(0, qunatityWithUnit.IndexOf(' ')));
        }

        // get unit from string
        private string CutUnit(string qunatityWithUnit)
        {
            return qunatityWithUnit.Substring(qunatityWithUnit.IndexOf(' ') + 1);
        }
    }
}