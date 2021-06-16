import React from "react";

class EditView extends React.Component {
    constructor(props) {
        super(props);

        // state with form values
        this.state = {
            recipeName: "",
            recipeDescription: [],
            ingredients: [],
            ingredientName: "",
            ingredientQuantity: 0,
            ingredientUnit: ""
        };

        // bind handlers
        this.handleInputChange = this.handleInputChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
        this.handleAddIngredientButton =
            this.handleAddIngredientButton.bind(this);
    }

    // handle all input changes
    handleInputChange(event) {
        const target = event.target;

        // if description field, split it into lines and place in the array
        const value =
            target.type === "textarea"
                ? target.value.split("\n")
                : target.value;
        const name = target.name;

        this.setState({
            [name]: value
        });
    }

    // handle submit event
    handleSubmit(event) {
        // prevent default submit event
        event.preventDefault();

        // create a recipe object
        let recipe = {
            [this.state.recipeName]: {
                recipe: this.state.recipeDescription
            }
        };

        let ingredients = this.state.ingredients;

        // push ingredients
        for (
            let ingredientIndex = 0;
            ingredientIndex < ingredients.length;
            ingredientIndex++
        ) {
            // get ingredient's name
            let ingredientName = Object.keys(ingredients[ingredientIndex])[0];

            // add to the new recipe object
            recipe[this.state.recipeName][ingredientName] =
                ingredients[ingredientIndex][ingredientName];
        }

        // use Fetch API to send it to the backend server
        const requestOptions = {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(recipe)
        };
        fetch("/add", requestOptions);

        // redirect to main page
        window.location.href = "/";
    }

    // handle ingredient adding
    handleAddIngredientButton() {
        // if ingredient is not unique, do not add it
        if (!this.isIngredientUnique()) return;

        // else add ingredient to the ingredients list and clear input fields
        let ingredient = {
            [this.state.ingredientName]: this.state.ingredientQuantity
                .toString()
                .concat(" ", this.state.ingredientUnit)
        };

        this.setState({
            ingredients: [...this.state.ingredients, ingredient],
            ingredientName: "",
            ingredientQuantity: 0,
            ingredientUnit: ""
        });
    }

    // handle ingredient deletion
    handleDeleteButton(id, event) {
        // prevent default submit event
        event.preventDefault();
        
        // remove ingredient from array and update that array
        let ingredients = this.state.ingredients;
        ingredients.splice(id, 1);
        this.setState({
            ingredients: ingredients
        })
    }

    // checking whether ingredient is unique
    isIngredientUnique() {
        let ingredients = this.state.ingredients;

        for (
            let ingredientIndex = 0;
            ingredientIndex < ingredients.length;
            ingredientIndex++
        ) {
            // get ingredient's name
            let ingredientName = Object.keys(ingredients[ingredientIndex])[0];

            // split quantity and unit
            let unitWithQuantity =
                ingredients[ingredientIndex][ingredientName].split(" ");

            // if ingredient's name and unit are already present, do not add such ingredient
            if (
                ingredientName === this.state.ingredientName &&
                unitWithQuantity[1] === this.state.ingredientUnit
            )
                return false;
        }
        return true;
    }

    // ingredients table generator
    ingredientsTable() {
        let tableContent = [];
        let ingredients = this.state.ingredients;

        for (
            let ingredientIndex = 0;
            ingredientIndex < ingredients.length;
            ingredientIndex++
        ) {
            // get ingredient's name
            let ingredientName = Object.keys(ingredients[ingredientIndex])[0];

            // split quantity and unit
            let unitWithQuantity =
                ingredients[ingredientIndex][ingredientName].split(" ");

            tableContent.push(
                <tr>
                    <td>{ingredientName}</td>
                    <td>{unitWithQuantity[0]}</td>
                    <td>{unitWithQuantity[1]}</td>
                    <td style={{ textAlign: "end" }}>
                        <button
                            type="submit"
                            onClick={(event) => this.handleDeleteButton(ingredientIndex, event)}
                            className="btn btn-danger btn"
                        >
                            Delete
                        </button>
                    </td>
                </tr>
            );
        }

        return tableContent;
    }

    // new ingredient subform generator
    ingredientsForm() {
        return (
            <>
                <div>
                    <label>Ingredients:</label>
                    <table className="table table-border">
                        <thead className="table-light">
                            <tr>
                                <th>Name</th>
                                <th>Quantity</th>
                                <th colSpan="2">Unit</th>
                            </tr>
                        </thead>
                        <tbody>{this.ingredientsTable()}</tbody>
                    </table>
                </div>
                <div className="form-group">
                    <label className="form-label">Ingredient's name:</label>
                    <input
                        name="ingredientName"
                        type="text"
                        value={this.state.ingredientName}
                        onChange={this.handleInputChange}
                        className="form-control"
                    />
                </div>
                <div className="row">
                    <div className="col">
                        <div className="form-group">
                            <label className="form-label">Quantity:</label>
                            <input
                                name="ingredientQuantity"
                                type="number"
                                value={this.state.ingredientQuantity}
                                min="0"
                                step="0.1"
                                onChange={this.handleInputChange}
                                className="form-control"
                            />
                        </div>
                    </div>
                    <div className="col">
                        <div className="form-group">
                            <label className="form-label">Unit:</label>
                            <input
                                name="ingredientUnit"
                                type="text"
                                value={this.state.ingredientUnit}
                                onChange={this.handleInputChange}
                                className="form-control"
                            />
                        </div>
                    </div>
                </div>
            </>
        );
    }

    // buttons row generator
    buttons() {
        return (
            <div className="row">
                <div className="col">
                    <button
                        type="button"
                        onClick={this.handleAddIngredientButton}
                        className="btn btn-primary btn-lg"
                    >
                        Add ingredient
                    </button>
                </div>
                <div className="col" style={{ textAlign: "end" }}>
                    <button type="submit" className="btn btn-success btn-lg">
                        Add recipe
                    </button>
                    <a className="btn btn-danger btn-lg" href="/">
                        Cancel
                    </a>
                </div>
            </div>
        );
    }

    // render form
    render() {
        return (
            <form method="POST" onSubmit={this.handleSubmit}>
                <div className="form-group">
                    <label className="form-label">Name:</label>
                    <input
                        name="recipeName"
                        type="text"
                        className="form-control"
                        value={this.state.recipeName}
                        onChange={this.handleInputChange}
                    />
                </div>
                <div className="form-group">
                    <label className="form-label">Description:</label>
                    <textarea
                        name="recipeDescription"
                        style={{ height: "120px" }}
                        className="form-control"
                        value={this.state.recipeDescription.join("\n")}
                        onChange={this.handleInputChange}
                    />
                </div>
                {this.ingredientsForm()}
                {this.buttons()}
            </form>
        );
    }
}

export default EditView;
