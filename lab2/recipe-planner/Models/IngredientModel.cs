namespace recipe_planner.Models
{
    public class IngredientModel
    {
        public string Name { get; set; }        // name of the ingredient
        public float Quantity { get; set; }     // quantity of current ingredient in the recipe
        public string Unit { get; set; }        // unit in which quantity is described
    }
}