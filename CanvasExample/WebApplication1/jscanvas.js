var canvas_dict = {};

function add_canvas(name)
{
    var theCanvas = document.getElementById(name);
    var context = theCanvas.getContext("2d");

    canvas_dict[name] = context;
}

function get_canvas(name) {
    return canvas_dict[name];
}