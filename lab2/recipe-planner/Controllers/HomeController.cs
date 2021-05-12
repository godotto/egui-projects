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

        // list of ingredient models
        static List<IngredientModel> ingredients = new List<IngredientModel>();

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
            // create new recipe model and add it to the list
            // recipes.Add(new RecipeModel());

            // pass list of ingredients to view
            ViewBag.ingredients = ingredients;

            return View(new NewRecipeModelView());
        }

#nullable enable
        [HttpPost]
        public IActionResult AddRecipe(string recipeName, string? description)
        {
            // get new recipe model object and assign recipe's name
            var newRecipe = new RecipeModel();
            newRecipe.Name = recipeName;

            if (description != null)
            {
                // remove \r characters from line breaks and save splited description to new recipe object
                description = description.Replace('\r', '\0');
                newRecipe.Description = description.Split('\n').ToList();
            }
            else // if description was empty, assign empty list
                newRecipe.Description = new List<string>();

            // add ingredients to new recipe
            newRecipe.Ingredients = new List<IngredientModel>();
            foreach (var ingredient in ingredients)
                newRecipe.Ingredients.Add(ingredient);

            // clear list of ingredients, add recipe to the list and return to the main view
            ingredients.Clear();
            recipes.Add(newRecipe);
            return RedirectToAction("Index");
        }
#nullable disable

        [HttpPost]
        public IActionResult AddIngredient(string ingredientName, float quantity, string unit, string recipeName, string description)
        {
            // create new ingredient model and assign ingredient's name, quantity and unit
            var newIngredient = new IngredientModel();
            newIngredient.Name = ingredientName;
            newIngredient.Quantity = quantity;
            newIngredient.Unit = unit;

            // add ingredient to the list (only if it is uniqe)
            if (IsIngredientUnique(ingredientName, unit))
                ingredients.Add(newIngredient);

            // pass name and description of recipe to ViewBag
            TempData["recipeName"] = recipeName;
            TempData["description"] = description;

            // refresh view
            return RedirectToAction("NewRecipe");
        }

        [HttpPost]
        public IActionResult DiscardRecipe()
        {
            // clear ingredient list and return to main view
            ingredients.Clear();
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

        // check if ingredient is unique or not
        private bool IsIngredientUnique(string name, string unit)
        {
            foreach (var ingredient in ingredients)
            {
                // if name is not unique, but unit is different it is a valid ingredient
                if (ingredient.Name.Equals(name) && ingredient.Unit.Equals(unit))
                    return false;
            }

            return true;
        }
    }
}