#import <Cocoa/Cocoa.h>
#import "GLWindow.hpp"
#import <memory>
NSApplication* application;

@interface App : NSWindow <NSApplicationDelegate>{
    std::shared_ptr<GLWindow> appInstance;
}
@property (nonatomic, retain) NSOpenGLView* glView;
-(void) drawLoop:(NSTimer*) timer;
@end