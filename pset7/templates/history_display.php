<table class="table table-hover">
    <thead>
        <tr>
            <th>Transaction</th>
            <th>Date/Time</th>
            <th>Symbol</th>
            <th>Shares</th>
            <th>Price</th>
        </tr>
    </thead>
    <tbody>
    <?php
    foreach ($activities as $activity) {
        print("<tr>"); 
        print("<td>{$activity["transaction"]}</td>"); 
        print("<td>{$activity["time"]}</td>"); 
        print("<td>{$activity["symbol"]}</td>");
        print("<td>{$activity["shares"]}</td>");
        print("<td>$"); 
        print("{$activity["price"]}</td>");
        print("</tr>");
    }
    ?>
    </tbody>

</table>
