<?php

    // configuration
    require("../includes/config.php"); 
    
    $activities = query("SELECT CASE WHEN transaction = 0 THEN 'SELL' ELSE 'BUY' END transaction, time, shares, price, symbol FROM history where id=?", $_SESSION["id"]);
    render("history_display.php", ["activities" => $activities, "title" => "History"]);
?>
