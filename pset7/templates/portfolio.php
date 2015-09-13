<body align="center">
<ul class="nav nav-pills">
    <li><a href="quote.php">Quote</a></li>
    <li><a href="buy.php">Buy</a></li>
    <li><a href="sell.php">Sell</a></li>
    <li><a href="history.php">History</a></li>
    <li><a href="cngPwd.php">Change Password</a></li>
    <li><a href="logout.php">Logout</a></li>
</ul>
<table class="table table-hover">
    <thead>
        <tr>
            <th>Symbol</th>
            <th>Name</th>
            <th>Shares</th>
            <th>Price</th>
            <th>TOTAL</th>
        </tr>
    </thead>
    <tbody>
    <?php
        foreach ($positions as $position) {
            $total = number_format($position["price"]*$position["shares"], 2);
            print("<tr>"); 
            print("<td>{$position["symbol"]}</td>"); 
            print("<td>{$position["name"]}</td>"); 
            print("<td>{$position["shares"]}</td>");
            print("<td>$"); 
            print("{$position["price"]}</td>");
            print("<td>$"); 
            print("$total</td>");
            print("</tr>");
    }
    ?>
    <tr>
        <td colspan="4">CASH</td>
        <?php print("<td>{$_SESSION["cash"]}</td>"); ?>
    </tr>
    </tbody>
</table>
</body>
