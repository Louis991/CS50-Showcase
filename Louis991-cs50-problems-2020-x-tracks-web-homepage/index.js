/* Code for toggling a darkmode class (in styles.css) for the body. Based on: https://harlemsquirrel.github.io/css/javascript/2017/12/08/dark-light-mode-persistent-switcher.html */

/* Returns a boolean for the theme being requested, true if dark, false if light. */
function whichTheme()
{
    return document.cookie.match(/theme=dark/i) != null;
}

/* Sets the theme based on the cookie's data */
function setTheme()
{
    document.querySelector('body').className = whichTheme() ? 'darkmode' : 'lightmode';
}

/* Creates cookie that stores data about which theme is being requested. */
function setCookie()
{
    var body = document.querySelector('body');
    var currentClass = body.className;
    var newClass = (currentClass == 'darkmode' ? 'lightmode' : 'darkmode');
    body.className = newClass;

    document.cookie = 'theme=' + (newClass == 'darkmode' ? 'dark' : 'light');
    console.log('Current cookie: ' + document.cookie);
}