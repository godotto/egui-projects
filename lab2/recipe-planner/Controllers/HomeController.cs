using System;
using System.Collections.Generic;
using System.Linq;

using System.Text.Json;

using Microsoft.AspNetCore.Mvc;
using recipe_planner.Models;

namespace recipe_planner.Controllers
{
    public class HomeController : Controller
    {
        // list of recipe models
        private static List<RecipeModel> recipes = new List<RecipeModel>();

        // list of ingredient models
        private static List<IngredientModel> ingredients = new List<IngredientModel>();

        // object with JSON formatted recipes
        private static JsonElement recipesJson;

        public HomeController()
        {
            // if list is empty, read recipes from JSON file
            if (recipes.Count == 0)
                ReadRecipesFromFile();
        }

        public IActionResult Index()
        {
            return View(recipes);
        }

        public IActionResult NewRecipe()
        {
            // pass list of ingredients to view
            ViewBag.ingredients = ingredients;

            // set mode to add
            TempData["mode"] = "add";

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
                description = description.Replace("\r", "");
                newRecipe.Description = description.Split('\n').ToList();
            }
            else // if description was empty, assign empty list
                newRecipe.Description = new List<string>();

            // add ingredients to new recipe
            newRecipe.Ingredients = new List<IngredientModel>();
            foreach (var ingredient in ingredients)
                newRecipe.Ingredients.Add(ingredient);

            // clear list of ingredients, add recipe to the list, save it to JSON file and return to the main view
            ingredients.Clear();
            recipes.Add(newRecipe);
            WriteRecipesToFile();
            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult AddIngredient(string? ingredientName, float quantity, string? unit, string recipeName, string description)
        {
            // create new ingredient model and assign ingredient's name, quantity and unit
            var newIngredient = new IngredientModel();
            newIngredient.Name = ingredientName;
            newIngredient.Quantity = quantity;
            newIngredient.Unit = unit;

            // add ingredient to the list (only if it is uniqe and non-empty)
            if (ingredientName != null && unit != null && IsIngredientUnique(ingredientName, unit))
                ingredients.Add(newIngredient);

            // pass name and description of recipe to ViewBag
            TempData["recipeName"] = recipeName;
            TempData["description"] = description;

            // if edite mode is active, redirect to edit mode with recipe's ID
            if (Convert.ToString(TempData["mode"]) == "edit")
                return RedirectToAction("EditRecipe", new {id = TempData["id"]});
            else
                return RedirectToAction("NewRecipe");
        }
#nullable disable

        public IActionResult DeleteRecipe(int id)
        {
            // only if ID is in range remove chosen recipe from the list and update JSON file
            if (id < recipes.Count && id >= 0)
            {
                recipes.RemoveAt(id);
                WriteRecipesToFile();
            }

            return RedirectToAction("Index");
        }

        [HttpPost]
        public IActionResult DiscardRecipe()
        {
            // clear ingredient list and return to main view
            ingredients.Clear();
            return RedirectToAction("Index");
        }

        public IActionResult EditRecipe(int id)
        {
            // if recipe does not exist stay in the main view
            if (id >= recipes.Count || id < 0)
                return RedirectToAction("Index");

            // create view model and fill it with recipe's data
            var editViewModel = new NewRecipeModelView();
            editViewModel.RecipeName = recipes[id].Name;
            editViewModel.Description = String.Join("\n", recipes[id].Description);

            if (ingredients.Count == 0)
            {
                foreach (var ingredient in recipes[id].Ingredients)
                    ingredients.Add(new IngredientModel(ingredient));
            }

            // pass list of ingredients to view
            ViewBag.ingredients = ingredients;

            // set mode to edit and pass ID to TempData
            TempData["mode"] = "edit";
            TempData["id"] = id;

            return View("NewRecipe", editViewModel);
        }

#nullable enable
        [HttpPost]
        public IActionResult EditRecipe(string recipeName, string? description)
        {
            // recipe to edit
            var editedRecipe = recipes[Convert.ToInt32(TempData["id"])];
            
            // replace name of the recipe and its description
            editedRecipe.Name = recipeName;

            if (description != null)
            {
                // remove \r characters from line breaks and save splited description to new recipe object
                description = description.Replace("\r", "");
                editedRecipe.Description = description.Split('\n').ToList();
            }
            else // if description was empty, assign empty list
                editedRecipe.Description = new List<string>();

            // update ingredients
            editedRecipe.Ingredients.Clear();
            foreach (var ingredient in ingredients)
                editedRecipe.Ingredients.Add(ingredient);

            // clear list of ingredients, add recipe to the list, save it to JSON file and return to the main view
            ingredients.Clear();
            WriteRecipesToFile();
            return RedirectToAction("Index");
        }
#nullable disable

        // read recipes from file
        private void ReadRecipesFromFile()
        {
            // read JSON file and deserialize it
            var jsonString = System.IO.File.ReadAllText("recipes.json");
            recipesJson = JsonSerializer.Deserialize<JsonElement>(jsonString);

            // fill list of models in loop
            foreach (var recipeJson in recipesJson.EnumerateObject())
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

        // write recipes to file
        private void WriteRecipesToFile()
        {
            // enable pretty printing
            var serializerOptions = new JsonSerializerOptions
            {
                WriteIndented = true
            };

            // dictionary representing whole JSON file with name of recipe as key and descrpition with ingredients as values
            var recipesDictionary = new Dictionary<string, Dictionary<string, dynamic>>();

            // read each recipe from the list
            foreach (var recipe in recipes)
            {
                // dictionary representing description with array of string lines or ingredient with quantity and unit
                var recipeDictionary = new Dictionary<string, dynamic>();

                // add description
                recipeDictionary.Add("recipe", recipe.Description);

                // iterate through each ingredient and add it to dictionary
                foreach (var ingredient in recipe.Ingredients)
                    recipeDictionary.Add(ingredient.Name, Convert.ToString(ingredient.Quantity) + ' ' + ingredient.Unit);

                // add recipe ditionary to the main dictionary
                recipesDictionary.Add(recipe.Name, recipeDictionary);
            }

            // serialize dictionary and save it to JSON file
            var jsonString = JsonSerializer.Serialize(recipesDictionary, serializerOptions);
            System.IO.File.WriteAllText("recipes.json", jsonString);
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