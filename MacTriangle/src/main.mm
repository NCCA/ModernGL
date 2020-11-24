// example based on https://riptutorial.com/opengl/example/13875/creating-opengl-4-1-with-cplusplus-and-cocoa
#import "App.h"

@implementation App

@synthesize glView;

BOOL shouldStop = NO;

int main(int argc, const char * argv[]) 
{
    App* app;
    application = [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular]; 
    //create a window with the size of 600 by 600   
    app = [[App alloc] initWithContentRect:NSMakeRect(0, 0, 1024, 720)              styleMask:NSWindowStyleMaskClosable  | NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |NSWindowStyleMaskMiniaturizable backing:NSBackingStoreBuffered defer:YES];    
    [application setDelegate:app];
    [application run];
}


-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{
  if(self = [super initWithContentRect:contentRect styleMask:aStyle backing:bufferingType defer:flag])
  {

    //This is pretty important.. OS X starts always with a context that only supports openGL 2.1
    //This will ditch the classic OpenGL and initialises openGL 4.1
    NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
    {
      NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
      NSOpenGLPFAColorSize    , 24                           ,
      NSOpenGLPFAAlphaSize    , 8                            ,
      NSOpenGLPFADoubleBuffer ,
      NSOpenGLPFAAccelerated  ,
      NSOpenGLPFANoRecovery   ,
      0
    };

    NSOpenGLPixelFormat* format = [[NSOpenGLPixelFormat alloc]initWithAttributes:pixelFormatAttributes];
    //Initialize the view 
    glView = [[NSOpenGLView alloc]initWithFrame:contentRect pixelFormat:format];
    //Set context and attach it to the window
    [[glView openGLContext]makeCurrentContext];

    [self setContentView:glView];
    [glView prepareOpenGL];
    [self makeKeyAndOrderFront:self];
    [self setAcceptsMouseMovedEvents:YES];
    [self makeKeyWindow];
    [self setOpaque:YES];
    //Start the c++ code
    appInstance = std::shared_ptr<GLWindow>(new GLWindow());

  } 
  return self;
}

-(void) drawLoop:(NSTimer*) timer
{
  if(shouldStop)
  {
    [self close];
    return;
  }
  if([self isVisible])
  {
    appInstance->update();
    [glView update];
    [[glView openGLContext] flushBuffer];
  }
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification 
{
  [NSTimer scheduledTimerWithTimeInterval:0.000001 target:self selector:@selector(drawLoop:) userInfo:nil repeats:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
  return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
  shouldStop = YES;
}

- (void)keyDown:(NSEvent *)theEvent 
{
    [self interpretKeyEvents:[NSArray arrayWithObject:theEvent]];
    NSString *chars = theEvent.charactersIgnoringModifiers;
    unichar aChar = [chars characterAtIndex: 0];
    if (theEvent.keyCode == 53)
        shouldStop=YES;
    else if (aChar == 'w')
      appInstance->setWireframe();
   else if (aChar == 's')
      appInstance->setSolid();

}


@end
