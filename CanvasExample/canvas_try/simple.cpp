#include <cstdio>
#include <emscripten.h>

/*
EM_JS(void, call_alert, (), {
  alert('hello world!');
  throw 'all done';
});
*/


int main()
{
    //call_alert();
	
	EM_ASM_({
		alert('I received: ' + UTF8ToString($0) + UTF8ToString($0));
	}, "Hello");
	
    return 0;
}


/*
int main()
{
	EM_ASM({
		var theCanvas = document.getElementById("canvas");
		var context = theCanvas.getContext("2d");

        //background
        context.fillStyle = "#ffffaa";
        context.fillRect(0, 0, 500, 300);

		context.fillStyle  = "#000000";
        context.font = "20px Sans-Serif";
        context.textBaseline = "top";
        context.fillText  ("Hello World!", 195, 80 );
	});
    return 0;
}
*/