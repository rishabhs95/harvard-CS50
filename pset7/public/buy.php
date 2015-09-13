<?php

    // configuration
    require("../includes/config.php"); 
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("buy_form.php", ["title" => "Buy"]);
    }
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide a stock's symbol to sell!");
        }
        else if (empty($_POST["shares"]))
            {
                apologize("You must provide number of stocks to buy!");
            }
        else if (!preg_match("/^\d+$/", $_POST["shares"]))
            {
                apologize("You must provide positive integral number of stocks to buy!");
            }
        $stockDetails = lookup($_POST["symbol"]);
        if ($stockDetails === false)
            {
                apologize("Entered stock symbol was invalid.");
            }
        else
        {
             $subtractAmount = $stockDetails["price"] * $_POST["shares"];
             if ($_SESSION["cash"] < $subtractAmount)
                {
                    apologize("You don't have sufficient amount of deposited money.");
                }
            else
            {
                query("INSERT INTO demat (id, symbol, shares) VALUES(?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + VALUES(shares)", $_SESSION["id"], $_POST["symbol"], $_POST["shares"]);
                query("UPDATE users SET cash = cash - ? WHERE id = ?", $subtractAmount, $_SESSION["id"]);
                query("INSERT INTO history (id, transaction, symbol, shares, price, time) VALUES(?, 1, ?, ?, ?, Now())", $_SESSION["id"], $_POST["symbol"], $_POST["shares"], number_format($stockDetails["price"], 2));
                $_SESSION["cash"] -= $subtractAmount;
            }
        }
    redirect("/");   
    }
?>
