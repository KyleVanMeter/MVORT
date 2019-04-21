* What I wanted to do by this deadline:

By this deadline I wanted to have the GUI finished.

* What I achieved

Looking at the git log does not tell anywhere near the full story here as so much mock-up code was thrown out.  I started out by having a simple openGL view with a fullscreen/fullview textured quad.  The idea was to simply update the textured quad as the image was being rendered.  However, this was scrapped as it seemed like over-kill to bring in a giant dependency like OpenGL, and learning openGL (and its interface with Qt) is already a monumental task.  Although there are some issues blocking this being complete I ended up re-doing the UI which now uses QThreads to render the pixels, and send them to the main UI thread which (should) draw them on the screen as a QPixmap.  There is some breakdown between the information sent by the QThread to the controller/UI Thread, and what is being put in the QImage/displayed; this is something I am still working on, but the bulk of the work is (hopefully) completed here.

* What I will do by next deadline

Acceleration structures.
