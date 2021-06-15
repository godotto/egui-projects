import React from "react";

class EditView extends React.Component {
    constructor(props) {
        super(props);
        
        // state with form values
        this.state = {
            recipeName: "",
            recipeDescription: [],
            ingredients: []
        };

        // bind handlers
        this.handleInputChange = this.handleInputChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
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

    handleSubmit(event) {
        // prevent default submit event
        event.preventDefault();

        // create a recipe object
        let recipe = {
            [this.state.recipeName]: {
                recipe: this.state.recipeDescription
            }
        };

        // use Fetch API to send it to the backend server
        const requestOptions = {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(recipe)
        };
        fetch("/add", requestOptions);
    }

    // buttons row generator
    buttons() {
        return (
            <div className="row">
                <div className="col">
                    {/* <button className="btn btn-primary btn-lg">
                        Add ingredient
                    </button> */}
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
                    <label>Name:</label>
                    <input
                        name="recipeName"
                        type="text"
                        className="form-control"
                        value={this.state.recipeName}
                        onChange={this.handleInputChange}
                    />
                </div>
                <div className="form-group">
                    <label>Description:</label>
                    <textarea
                        name="recipeDescription"
                        style={{ height: "120px" }}
                        className="form-control"
                        value={this.state.recipeDescription.join("\n")}
                        onChange={this.handleInputChange}
                    />
                </div>
                <div />
                {this.buttons()}
            </form>
        );
    }
}

export default EditView;
