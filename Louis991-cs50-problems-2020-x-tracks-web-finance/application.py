import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import apology, login_required, lookup, usd
import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # Query database for symbol and cumulative "bought" (+ for purchases, - for sales) for the current user stored in sessions.
    rows = db.execute("SELECT symbol, SUM(bought) FROM transactions WHERE id = :id GROUP BY symbol", id=session["user_id"])

    # Lists to store company names, their current stock prices and the user's holdings. All monetary values are treated and manipulated as integers and expressed as cents
    companyNames = []
    stockPrices = []
    holdingValues = []
    holdingValuesString = []

    for row in rows:
        # Obtains name, price and symbol.
        currentInfo = lookup(row["symbol"])
        currentInfo["price"] = int(currentInfo["price"] * 100)

        # Adds info to the lists.
        companyNames.append(currentInfo["name"])
        stockPrices.append(usd(currentInfo["price"] / 100))
        holdingValues.append(currentInfo["price"] * row["SUM(bought)"])
        holdingValuesString.append(usd((currentInfo["price"] * row["SUM(bought)"]) / 100))

    # Obtain the current user's balance.
    currentCashDict = db.execute("SELECT cash FROM users WHERE users.id = :id", id=session["user_id"])[0]
    currentCashString = usd(currentCashDict["cash"]/100)

    # User's grand total.
    grandSum = 0
    for holding in holdingValues:
        grandSum += holding
    grandTotal = usd((grandSum + currentCashDict["cash"])/100)

    # Length of the rows list.
    rowsLength = len(rows)

    # Render the index table
    return render_template("index.html", rows=rows, companyNames=companyNames, stockPrices=stockPrices, holdingValuesString=holdingValuesString, currentCashString=currentCashString, grandTotal=grandTotal, rowsLength=rowsLength)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":

        # Input check - empty symbol input.
        if not request.form.get("symbol"):
            return apology("Enter a stock's symbol.")

        # Input check - no number of shares.
        if not request.form.get("shares"):
            return apology("Enter a number of shares to buy.")

        # Input check -  Negative share number.
        if int(request.form.get("shares")) <= 0:
            return apology("Only positive integer numbers are allowed when buying shares.", 403)
        else:
            shareNumber = int(request.form.get("shares"))

        # Obtain symbol object (contains name, price and symbol of the stock to be bought), call apology if it's not returned.
        if lookup(request.form.get("symbol")) != None:
            sym = lookup(request.form.get("symbol"))
            # Convert stock's price from float to integer type (cents)
            sym["price"] = int(sym["price"] * 100)
        else:
            return apology("The stock symbol doesn't exist.", 403)

        # Current amount of cash for the user
        currentCashDict = db.execute("SELECT cash FROM users WHERE users.id = :id", id=session["user_id"])[0]
        currentCash = currentCashDict["cash"]

        # If cash is insufficient to make the purchase.
        if currentCash < shareNumber * (sym["price"]):
            return apology("You don't have enough cash to make this transaction.", 403)

        # Else, make the purchase.
        else:
            transactionCost = shareNumber * sym["price"]

            # Deduct purchase amount from cash in the users table.
            db.execute("UPDATE users SET cash = cash - :amount WHERE users.id = :id", amount=transactionCost, id=session["user_id"])

            # Register the operation into the transactions table.
            db.execute("INSERT INTO transactions (id, symbol, bought, stockPrice, balance) values (:idTr, :symbol, :bought, :stockPrice, :balance)", idTr=session["user_id"], symbol=sym["symbol"], bought=shareNumber, stockPrice=usd(sym["price"]/100), balance=usd((currentCash-transactionCost)/100))

            #Notify the user that the purchase was successful.
            flash("Your purchase was successful.")
            return render_template("buy.html")

    # If GET method.
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Extract transaction history of the user.
    rows = db.execute("SELECT symbol, bought, stockPrice, date FROM transactions WHERE transactions.id = :idTr", idTr=session["user_id"])

    # rowsLength -> number of dictionaries returned by db.execute(). Each dictionary represents the data for each row.
    rowsLength = len(rows)

    # List with the names of every company the user has bought stock from.
    companyNames = []
    for row in rows:
        companyInfo = lookup(row["symbol"])
        companyNames.append(companyInfo["name"])

    # Send information to the history template and render it.
    return render_template("history.html", rows=rows, rowsLength=rowsLength, companyNames=companyNames)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("You must provide an username.", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("You must provide a password.", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid username and/or password.", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        sym = lookup(request.form.get("symbol"))
        price = usd(sym["price"])
        return render_template("quoted.html", sym = sym, price=price)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":

        # No username.
        if not request.form.get("username"):
            return apology("Insert an username.", 403)

        # If username already exists.
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) == 1:
            return apology("This username already exists.", 403)

        # Missing password or confirmation.
        if not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Insert a password and its confirmation.", 403)

        # Password and its confirmation don't match.
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("Password and its confirmation don't match.", 403)

        # If all previous checks fail, register the new user and the corresponding hash.
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))

        # Redirect to the login route.
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # Extract the user's investments in a dictionary -> investments = {"symbol":numberOfShares, ...}
    # {"AAPL":10, "CSCO":5, ...}
    investments = {}
    rows = db.execute("SELECT symbol, SUM(bought) FROM transactions WHERE id = :id GROUP BY symbol", id=session["user_id"])
    for row in rows:
        investments[row["symbol"]] = row["SUM(bought)"]

    if request.method == "POST":

        # Input checking - User doesn't select a stock.
        if not request.form.get("symbol"):
            return apology("Please select a stock's symbol.", 403)

        # Input checking - User doesn't own any shares from the stock selected or number of shares isn't greater than 0.
        if not investments[request.form.get("symbol")] or not investments[request.form.get("symbol")] > 0:
            return apology("You don't own any shares from the selected stock.", 403)

        # Input checking - User doesn't input a number of shares to sell.
        if not request.form.get("shares"):
            return apology("Please enter a number of shares to sell.", 403)

        # If user attempts to sell more shares than he owns.
        if investments[request.form.get("symbol")] < int(request.form.get("shares")):
            return apology("You don't own enough shares to make this sale.", 403)

        # Price of the stock to be sold, in cents, as an integer.
        stockInfo = lookup(request.form.get("symbol"))
        stockPrice = int(stockInfo["price"] * 100)

        # Cash gained from the sale, in cents, as an integer.
        cashGained = int(request.form.get("shares")) * stockPrice

        # Add cash gained to the user's balance.
        db.execute("UPDATE users SET cash = cash + :cashGained WHERE users.id = :idTr", cashGained=cashGained, idTr=session["user_id"])

        # Extract the user's cash balance
        currentCashDict = db.execute("SELECT cash FROM users WHERE users.id = :idTr", idTr=session["user_id"])[0]
        currentCash = currentCashDict["cash"]

        # Register the transaction.
        db.execute("INSERT INTO transactions (id, symbol, bought, stockPrice, balance) values (:id, :symbol, :bought, :stockPrice, :balance)", id=session["user_id"], symbol=request.form.get("symbol"), bought=int(request.form.get("shares"))*(-1), stockPrice=usd(stockPrice/100), balance=usd((currentCash+cashGained)/100))

        #Notify the user that the sale was successful.
        flash("Your sale was successful.")
        return redirect("/")

    else:
        return render_template("sell.html", investments=investments)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)