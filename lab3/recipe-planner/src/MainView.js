import React from "react";

class MainView extends React.Component {
    // generate list of recipes with buttons
    table() {
        let tableContent = [];

        for (
            let recipeIndex = 0;
            recipeIndex < this.props.recipes.length;
            recipeIndex++
        ) {
            tableContent.push(
                <tr>
                    <td>{this.props.recipes[recipeIndex]}</td>
                    <td style={{ textAlign: "end" }}>
                        <div class="btn-group">
                            <button
                                type="button"
                                className="btn btn-success btn"
                            >
                                Edit
                            </button>
                            <button
                                type="button"
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
                <button className="btn btn-outline-primary btn-lg">Add</button>
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
