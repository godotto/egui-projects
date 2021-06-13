import React from "react";
import MainView from "./MainView";

class App extends React.Component {
    constructor(props) {
        // call Component's constructor
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

    // render app view
    render() {
        // select content based on route
        let content;
        if (window.location.pathname === "/") {
            content = <MainView recipes={this.state.recipes} />;
        }

        return (
            <>
                <header>
                    <nav className="navbar navbar-expand-sm navbar-toggleable-sm navbar-light bg-white border-bottom box-shadow mb-3">
                        <div className="container">Recipe planner</div>
                    </nav>
                </header>
                <div className="container">
                    <main role="main" className="pb-3">
                        {content}
                    </main>
                </div>
            </>
        );
    }
}

export default App;
