<form action="sell.php" method="post">
    <fieldset>
        <div class="form-group">
            <select class="form-control" name="symbol">
                <?php
                    foreach ($sellOptions as $sellOption) {
                        print("<option value='{$sellOption["symbol"]}'>{$sellOption["symbol"]}</option>"); 
                }
                ?>
            </select>
        </div>
        <div class="form-group">
            <button type="submit" class="btn btn-default">Sell</button>
        </div>
    </fieldset>
</form>
