from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    cash = db.execute("SELECT cash from users where id = :user_id", user_id = session["user_id"])
    info = db.execute("SELECT * from portfolios WHERE user_id = :user_id", user_id = session["user_id"])
    total = cash[0]["cash"]
    for position in info:
        position['info'] = lookup(position['symbol'])
        value = position['info']['price'] * position['shares']
        total += value
        position['info']['value'] =  usd(value)
    return render_template("index.html", cash = usd(cash[0]["cash"]), info = info, total = usd(total))
    return apology("TODO")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be integer")
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif shares <= 0:
            return apology("shares must be positive integer")
        else:
            stock = lookup(request.form.get("symbol"))
            if(stock == None):
                return apology("Please provide a valid symbol.")
            else:
                price = stock["price"]
                cash = db.execute("SELECT cash from users where id = :user_id", user_id = session["user_id"])
                debt = price * shares
                if (debt > cash[0]["cash"]):
                    return apology("You don't have enough cash.")
                else:
                    row = db.execute("select * from portfolios WHERE user_id = :user_id and symbol = :symbol", user_id = session["user_id"], symbol = stock["symbol"])
                    if len(row) != 0:    
                        update_row = db.execute("UPDATE portfolios set shares = shares + :add_shares WHERE user_id = :user_id and symbol = :symbol", user_id = session["user_id"],symbol = stock["symbol"], add_shares = shares)
                    else:
                        db.execute("INSERT OR IGNORE INTO portfolios (user_id, symbol, shares) VALUES(:user_id, upper(:symbol), :shares)", user_id = session["user_id"],symbol = stock["symbol"], shares = shares)
                        #db.execute("REPLACE INTO portfolios (user_id, symbol, shares) VALUES(:user_id, upper(:symbol), shares+ :add_shares)", user_id = session["user_id"],symbol = stock["symbol"], add_shares = shares)
                    db.execute("UPDATE users SET cash = cash - :debt WHERE id = :user_id", debt = debt, user_id = session["user_id"])
                    db.execute("INSERT INTO history (user_id, history, time, symbol, shares, price) VALUES (:user_id, :transaction, datetime('now'),:symbol, :shares, :price)", user_id = session["user_id"], transaction = 'BUY' ,symbol = stock["symbol"],shares = shares,price = stock["price"])
                    flash("You bought {} shares of stock {} for {} per stock!".format(shares, stock["symbol"], usd(price)))
                    return redirect("/");
    else:
        cash = db.execute("SELECT cash from users where id = :user_id", user_id = session["user_id"])
        info = db.execute("SELECT * from portfolios WHERE user_id = :user_id", user_id = session["user_id"])
        total = cash[0]["cash"]
        for position in info:
            position['info'] = lookup(position['symbol'])
            value = position['info']['price'] * position['shares']
            total += value
            position['info']['value'] =  usd(value)
        return render_template("buy.html", cash = usd(cash[0]["cash"]), info = info, total = usd(total))
        #return render_template("buy.html", info = info)
    return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    user_info = db.execute("SELECT * FROM history WHERE user_id = :user_id", user_id = session["user_id"])
    return render_template("history.html", info = user_info)
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if request.form.get("symbol")!= None and lookup(request.form.get("symbol")) != None:
            return render_template("quoted.html", quoted = lookup(request.form.get("symbol")))
        else:
            return apology("Please input a valid symbol")
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    #return apology("TODO")

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
            
        # ensure password(again) was submitted
        elif not request.form.get("passwordVeri"):
            return apology("must verify password")

        # query database for inserting username and password
        rows = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hashValue)", username = request.form.get("username"), hashValue = pwd_context.encrypt(request.form.get("password")))
        print(rows)
        # ensure username exists and password is correct
        if rows == None:
            return apology("invalid username and/or password")

        # redirect user to home page
        # remember which user has logged in
        session["user_id"] = rows
        flash("You are successfully registered")
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be integer")
        if not request.form.get("symbol"):
            return apology("must provide symbol")
        elif shares <= 0:
            return apology("shares must be positive integer")
        else:
            stock = lookup(request.form.get("symbol"))
            price = stock["price"]
            if(len(stock) != 3):
                
                return apology("Please provide a valid symbol.")
            else:
                current_shares = db.execute("SELECT shares from portfolios WHERE user_id = :user_id and symbol = :symbol", user_id = session["user_id"], symbol = stock["symbol"])
                #print(current_shares[0])
                #cash = db.execute("SELECT cash from users where id = :user_id", user_id = session["user_id"]) 
                if current_shares == None:
                    return apology("You don't have any shares of this stock.")
                elif (current_shares[0]['shares'] < shares):
                    return apology("You don't have enough shares.")
                else:
                    profit = price * shares   # profit changed by debt 
                    db.execute("UPDATE users set cash = cash + :profit WHERE id = :user_id", user_id = session["user_id"], profit = profit)
                    db.execute("UPDATE portfolios set shares = shares - :reduce_shares WHERE user_id = :user_id and symbol = :symbol", user_id = session["user_id"],symbol = stock["symbol"], reduce_shares = shares)
                    #db.execute("UPDATE users SET cash = cash - :debt WHERE id = :user_id", debt = debt, user_id = session["user_id"])
                    db.execute("INSERT INTO history (user_id, history, time, symbol, shares, price) VALUES (:user_id, :transaction, datetime('now'),:symbol, :shares, :price)", user_id = session["user_id"], transaction = 'SELL' ,symbol = stock["symbol"],shares = shares,price = stock["price"])
                    flash("You sold {} shares of the stock {} with ${} per stock!".format(shares,stock["symbol"], price))
                    return redirect("/");
    else:
        info = db.execute("SELECT * from portfolios WHERE user_id = :user_id", user_id = session["user_id"])
        for position in info:
            position['info'] = lookup(position['symbol'])
            value = position['info']['price'] * position['shares']
            position['info']['value'] = usd(value)
        return render_template("sell.html", info = info)
    return apology("TODO")
