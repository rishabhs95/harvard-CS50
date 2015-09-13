<?php
     // configuration
require("../includes/config.php");
     // if user reached page via GET (as by clicking a link or via redirect)
if ($_SERVER["REQUEST_METHOD"] == "GET") {
         // else render form
         render("change_password.php", ["title" => "Change Password"]);
     }
     // else if user reached page via POST (as by submitting a form via POST)
else if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // TODO
        // validate submission
        if (empty($_POST["old_password"]))
        {
            apologize("You must provide your old password.");
        }
        else if (empty($_POST["new_password"]))
        {
            apologize("You must provide a new password.");
        }
        else if (empty($_POST["new_confirmation"]))
        {
            apologize("You must provide a confirmation password");
        }
        else if ($_POST["new_password"] != $_POST["new_confirmation"])
        {
            apologize("Your password and confirmation password must match!");
        }
        
        $rows = query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
       
        if (crypt($_POST["old_password"], $rows[0]["hash"]) == $rows[0]["hash"])
        // else change password
        {
            query("UPDATE users SET hash = ? where id = ?", crypt($_POST["new_password"]), $_SESSION["id"]);
            
            // redirect to index
            redirect("/index.php");
        }
}
?>
