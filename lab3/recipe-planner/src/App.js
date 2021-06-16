import React from "react";
import MainView from "./MainView";
import EditView from "./EditView";

class App extends React.Component {
    // render app view
    render() {
        // select content based on route
        let content;
        if (window.location.pathname === "/") {
            content = <MainView />;
        }
        else if (window.location.pathname === "/new_recipe") {
            content = <EditView mode="0" />;
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
