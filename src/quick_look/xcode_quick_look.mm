#include <QtCore/QDir>
#include <QtWidgets/QAbstractItemView>
#import <objc/runtime.h>
#import <Quartz/Quartz.h>

#include "quick_look.h"


static QAbstractItemView *itemView = 0;


@interface NSView ( NSViewQuickPreview ) <QLPreviewPanelDataSource, QLPreviewPanelDelegate>

- (QAbstractItemView *)view;
- (void)setView:(QAbstractItemView *)view;
- (BOOL)acceptsPreviewPanelControl:(QLPreviewPanel *)panel;
- (void)beginPreviewPanelControl:(QLPreviewPanel *)panel;
- (void)endPreviewPanelControl:(QLPreviewPanel *)panel;

@property QAbstractItemView *view;

@end


@implementation NSView ( NSViewQuickPreview )

@dynamic view;


- (QAbstractItemView *)view
{
    return itemView;
}

- (void)setView:(QAbstractItemView *)view
{
    itemView = view;
}

- (BOOL)acceptsPreviewPanelControl:(QLPreviewPanel *)panel
{
    if(global_variable_quick_look_preview_close_status_bool)
        return NO;

    Q_UNUSED(panel)

    return self.view ? YES : NO;
}


- (void)beginPreviewPanelControl:(QLPreviewPanel *)panel
{
    if(global_variable_quick_look_preview_close_status_bool)
        return;

    panel.dataSource = self;
    panel.delegate = self;
    panel.currentPreviewItemIndex = self.view->currentIndex().row();
}


- (void)endPreviewPanelControl:(QLPreviewPanel *)panel
{

    Q_UNUSED(panel)
    // self.view->setCurrentIndex( self.view->model()->index( panel.currentPreviewItemIndex, 0 ) );

    self.view = nil;
    global_variable_quick_look_preview_close_status_bool = true;


    if([[QLPreviewPanel sharedPreviewPanel] isVisible])
        [[QLPreviewPanel sharedPreviewPanel] orderOut:nil];

}

- (NSInteger)numberOfPreviewItemsInPreviewPanel:(QLPreviewPanel *)panel
{
    if(global_variable_quick_look_preview_close_status_bool)
        return 0;

    Q_UNUSED(panel)
    return self.view->model()->rowCount();
}


- (id <QLPreviewItem>)previewPanel:(QLPreviewPanel *)panel previewItemAtIndex:(NSInteger)index
{
    Q_UNUSED(panel)

    NSString *filePath_str =  global_variable_quick_look_preview_filepath_QString.toNSString();

    NSArray *files = [filePath_str componentsSeparatedByString:@""];

    NSURL *fileURL = nil;

    fileURL = [NSURL fileURLWithPath:filePath_str];

    return fileURL;

}



@end



void quick_look::setPreviewIndex( const QModelIndex &index )
{
    if ([[QLPreviewPanel sharedPreviewPanel] isVisible])
    {
        [QLPreviewPanel sharedPreviewPanel].currentPreviewItemIndex = index.row();
    }
}

void quick_look::showPreview()
{
    NSView *view = (__bridge NSView*)(void*)winId();

    view.view = this;  //Function calling for  (void)setView:(QAbstractItemView *)view
    if ([[QLPreviewPanel sharedPreviewPanel] isVisible])
    {
        hidePreview();
    }

    [[QLPreviewPanel sharedPreviewPanel] makeKeyAndOrderFront:view]; //Calling of Function - (BOOL)acceptsPreviewPanelControl:(QLPreviewPanel *)panel
}

void quick_look::pub_close_view()
{

    NSView *view = (__bridge NSView*)(void*)winId();
    view = nil;

    if ([[QLPreviewPanel sharedPreviewPanel] isVisible])
        [[QLPreviewPanel sharedPreviewPanel] orderOut:nil];
}

void quick_look::hidePreview()
{
    [[QLPreviewPanel sharedPreviewPanel] makeKeyAndOrderFront:nil];
    [[QLPreviewPanel sharedPreviewPanel] reloadData];
}

