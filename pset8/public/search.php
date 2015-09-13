<?php

    require(__DIR__ . "/../includes/config.php");

    // numerically indexed array of places
    $places = [];
    
    // searching database for places matching $_GET["geo"]
    $keywords = preg_split("/[\s,]+/", $_GET["geo"]);
    $number = count($keywords);
    $changed = false;
    if ($number == 1 && is_numeric($keywords[0]))
    {
        $places = query("SELECT * FROM places WHERE postal_code = ?", $keywords[0]);
    }
    else
    {
        for ($index = 0; $index < $number; $index++)
        {
            if (strlen($keywords[$index]) <= 3)
            {
                if (is_numeric($keywords[$index]))
                {
                    strcpy($keywords[$index], query("SELECT admin_name2 FROM 
                    places WHERE admin_code2 = ?", $keywords[$index]));
                }
                else
                {
                    strcpy($keywords[$index], query("SELECT admin_name1 FROM 
                    places WHERE admin_code1 = ?", $keywords[$index]));
                }
                $changed = true;
            }
        }
        if ($changed == true)
        {
            $changedQuery = implode("+", $keywords);
            $places = query("SELECT * FROM `places` WHERE MATCH (`postal_code`, 
            `place_name`, `admin_name1`, `admin_name2`) AGAINST (?)", $changedQuery);
        }
        else
        {
            $places = query("SELECT * FROM `places` WHERE MATCH (`postal_code`, 
            `place_name`, `admin_name1`, `admin_name2`) AGAINST (?)", $_GET["geo"]);
        }
    }
    // output places as JSON (pretty-printed for debugging convenience)
    header("Content-type: application/json");
    print(json_encode($places, JSON_PRETTY_PRINT));

?>
