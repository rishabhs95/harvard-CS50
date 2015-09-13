<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("quote_form.php", ["title" => "Get Quote"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a symbol!");
        }
        
        $stock = lookup($_POST["symbol"]);
        if ($stock === false)
        {
            apologize("The Symbol Entered is not valid!");
        }
        else
        {
            // render quote_display
            render("quote_display.php", ["symbol" => $stock["symbol"], "price" => $stock["price"], "name" => $stock["name"], "title" => "Quote"]);
        }
    }

?>
