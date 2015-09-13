/**
 * scripts.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Global JavaScript.
 */

// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [

        // hide Google's labels
        {
            featureType: "all",
            elementType: "labels",
            stylers: [
                {visibility: "off"}
            ]
        },

        // hide roads
        {
            featureType: "road",
            elementType: "geometry",
            stylers: [
                {visibility: "off"}
            ]
        }

    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 42.3770, lng: -71.1256}, // Cambridge, Massachusetts
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    var homeLatLng = new google.maps.LatLng(parseFloat(place.latitude), parseFloat(place.longitude));

    var marker = new MarkerWithLabel({
        position: homeLatLng,
        draggable: true,
        raiseOnDrag: true,
        map: map,
        labelContent: place.place_name + ", " + place.admin_code1,
        labelAnchor: new google.maps.Point(22, 0),
        labelClass: "label",
        labelStyle: {opacity: 0.75}
    });
    
    /*var parameters = { geo: place.postal_code };
    $.getJSON("articles.php", parameters)
     .done(function(data, textStatus, jqXHR) 
    {
        var articles = "<ul>";
        var len = data.length;

        for (var i = 0; i < len; i++) 
        {
            articles += "<div><li> <a href=\"" + data[i].link + "\" target=\"_blank\">" + data[i].title + "</li></div>";
        }
        articles += "</ul>";
        
        if(articles.length === 0)
        {
            articles = "Slow News Day!";
        }*/
        //showInfo(marker, articles);
        google.maps.event.addListener(marker, 'click', function() 
        { showFullInfo(marker, place.place_name + "," + place.admin_code1); });
    });
    markers.push(marker); 
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {
        update();
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // remove markers whilst dragging
    google.maps.event.addListener(map, "dragstart", function() {
        removeMarkers();
    });

    // configure typeahead
    // https://github.com/twitter/typeahead.js/blob/master/doc/jquery_typeahead.md
    $("#q").typeahead({
        autoselect: true,
        highlight: true,
        minLength: 1
    },
    {
        source: search,
        templates: {
            empty: "no places found yet",
            suggestion: _.template("<p><%- place_name %>, <%- admin_name1 %>, <%- postal_code %></p>")
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // ensure coordinates are numbers
        var latitude = (_.isNumber(suggestion.latitude)) ? suggestion.latitude : parseFloat(suggestion.latitude);
        var longitude = (_.isNumber(suggestion.longitude)) ? suggestion.longitude : parseFloat(suggestion.longitude);

        // set map's center
        map.setCenter({lat: latitude, lng: longitude});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        hideInfo();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Hides info window.
 */
function hideInfo()
{
    info.close();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    for (var i = 0; i < markers.length; i++) 
    {
        markers[i].setMap(null);
        markers[i] = null;
    }
    markers.length = 0;
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, cb)
{
    // get places matching query (asynchronously)
    var parameters = {
        geo: query
    };
    $.getJSON("search.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // call typeahead's callback with search results (i.e., places)
        cb(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) === "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='img/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * gets JSON of local weather
 */
function showFullInfo(marker, query)
{
    // get weather for query
    var parameters = { q: query };

    $.getJSON("http://api.openweathermap.org/data/2.5/weather", parameters)
    .done(function(data, textStatus, jqXHR) 
    {

        // build weather display data
        var weatherInfo;
        if (data) 
        {
            var temp_c = Math.round(data.main.temp - 273.15);
            var temp_f = Math.round((temp_c * 9) / 5 + 32);
            weatherInfo = "<p><b>Current Weather:</b><br/>" + temp_f + "&degF / " + temp_c + "&degC, " + data.weather[0].description + "</p>";
        } 
        else 
        {
            weatherInfo = "<p>(No weather data available)</p>";
        }

        // forward on to append news articles
        articles(marker, query, weatherInfo);
     })
     .fail(function(jqXHR, textStatus, errorThrown) 
     {
         // log error to browser's console
         console.log(errorThrown.toString());
     });
}


/**
 * gets JSON of news articles
 */
function articles(marker, query, weatherInfo)
{
    // get articles for query
    var parameters = { geo: query };

    $.getJSON("articles.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // build HTML list of articles for query
        var article = "";

        if (data.length > 0) 
        {
            article = "<ul>";

            for (var i = 0; i < data.length; i++) 
            {
                article += "<li><a href='" + data[i].link + "' target=_blank>" + data[i].title + "</a></li>";
            }

            article += "</ul>";
        } 
        else 
        {
            article = "No news available";
        }

        //showInfo(marker, articleList);
        showInfo(marker, weatherInfo + article);
     })
     .fail(function(jqXHR, textStatus, errorThrown) 
     {
         // log error to browser's console
         console.log(errorThrown.toString());
     });
};

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON("update.php", parameters)
    .done(function(data, textStatus, jqXHR) {

        // remove old markers from map
        removeMarkers();

        // add new markers to map
        for (var i = 0; i < data.length; i++)
        {
            addMarker(data[i]);
        }
     })
     .fail(function(jqXHR, textStatus, errorThrown) {

         // log error to browser's console
         console.log(errorThrown.toString());
     });
};
