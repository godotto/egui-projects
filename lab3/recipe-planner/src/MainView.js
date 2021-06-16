import React from "react";

class MainView extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            recipes: []
        };
    }

    componentDidMount() {
        // read fetch recipes
        this.readRecipeNames();
    }

    // fetch recipe names from backend server
    readRecipeNames() {
        fetch("/recipes.json")
            .then((res) => res.json())
            .then((json) => {
                let fetchedRecipes = [];

                for (const recipe in json) {
                    fetchedRecipes.push(recipe);
                }

                this.setState({ recipes: fetchedRecipes });
            });
    }

    // handle recipe deletion
    handleDeleteRecipeButton(id) {
        // remove ingredient from array and update that array
        let recipes = this.state.recipes;
        let recipeToDelete = recipes[id];

        recipes.splice(id, 1);
        this.setState({
            recipes: recipes
        });

        console.log(recipes);

        // use Fetch API to send new object to the backend server
        const requestOptions = {
            method: "POST",
            headers: { "Content-Type": "text/plain" },
            body: recipeToDelete
        };
        fetch("/delete", requestOptions);
    }

    // generate list of recipes with buttons
    table() {
        let tableContent = [];

        for (
            let recipeIndex = 0;
            recipeIndex < this.state.recipes.length;
            recipeIndex++
        ) {
            tableContent.push(
                <tr>
                    <td>{this.state.recipes[recipeIndex]}</td>
                    <td style={{ textAlign: "end" }}>
                        <div className="btn-group">
                            <a href={"/edit_recipe/?name=" + this.state.recipes[recipeIndex]} type="button" className="btn btn-success btn">
                                Edit
                            </a>
                            <button
                                type="button"
                                onClick={this.handleDeleteRecipeButton.bind(
                                    this,
                                    recipeIndex
                                )}
                                className="btn btn-danger btn"
                            >
                                Delete
                            </button>
                        </div>
                    </td>
                </tr>
            );
        }

        return tableContent;
    }

    // generate vertical menu
    menu() {
        return (
            <div className="btn-group-vertical">
                <a
                    className="btn btn-outline-primary btn-lg"
                    href="/new_recipe"
                >
                    Add
                </a>
                <button className="btn btn-outline-primary btn-lg">
                    Create menu
                </button>
            </div>
        );
    }

    // render app view
    render() {
        return (
            <div className="container">
                <div className="row">
                    <div className="col-9">
                        <table className="table table-hover table-border">
                            <tbody>{this.table()}</tbody>
                        </table>
                    </div>
                    <div className="col-3">{this.menu()}</div>
                </div>
            </div>
        );
    }
}

export default MainView;
