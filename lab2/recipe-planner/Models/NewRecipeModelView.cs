using System.Collections.Generic;

namespace recipe_planner.Models
{
    public class NewRecipeModelView   // model view for new recipe screen
    {
        public string RecipeName { get; set; }
        public string Description { get; set; }
        public string IngredientName { get; set; }
        public float Quantity { get; set; }
        public string Unit { get; set; }
    }
}