import logo from './logo.svg';
import './App.css';
import React, {useState, useEffect} from "react";
import {Button, FormControl} from "@mui/material";

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={"https://lh3.googleusercontent.com/p/AF1QipP_iCalyZrE2LX1BCDe46dSDJlFUtyE9AjzpZek=s680-w680-h510"} style={{ height: "25%", width: "25%" }} alt="logo" />
        <p>
          Boson <code></code> Terminal
        </p>
        <a
          className="App-link"
          href="https://cern.ch"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn About the terminal!
        </a>
      </header>
    </div>
  );
}

export default App;
