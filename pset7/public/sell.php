<?php

    // configuration
    require("../includes/config.php"); 
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        $sellOptions = query("SELECT symbol FROM demat where id = ?", $_SESSION["id"]);
        render("sellStock.php", ["sellOptions" => $sellOptions, "title" => "Sell"]);
    }
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["symbol"]))
        {
            apologize("You must provide your stock's symbol to sell!");
        }
        $row = query("SELECT shares from demat where id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        if ($row === false)
        {
            apologize("You must provide a valid stock's symbol you have already purchased!");
        }
        else
        {
            $number = $row[0]["shares"];
            $stockDetails = lookup($_POST["symbol"]);
            $price = number_format(floatval($stockDetails["price"]), 2);
            $addAmount = $number*$stockDetails["price"];
            $querya = query("UPDATE users SET cash = cash + ? WHERE id = ?", $addAmount, $_SESSION["id"]);
            $queryb = query("INSERT INTO history (id, transaction, symbol, shares, price, time) VALUES(?, ?, ?, ?, ?, Now())", $_SESSION["id"], 0, $_POST["symbol"], $_POST["shares"], $price);
            $queryc = query("DELETE FROM demat WHERE id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
            if ($querya && $queryb && $queryc === false)
                apologize("error!");
            $_SESSION["cash"] += $addAmount;
        }
        redirect("/");
    }    
?>
