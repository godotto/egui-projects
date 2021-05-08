using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace recipe_planner.Models
{
    public class RecipeModel
    {
        public string Name { get; set; }
        public List<IngredientModel> Ingredients { get; set; }
        
        [DataType(DataType.MultilineText)]
        public List<string> Description { get; set; }   // description of the recipe
    }
}