$.fn.fixedHeader = function (options) {
 var config = {
-   topOffset: 40
+   topOffset: 38
   //bgColor: 'white'
 };
 if (options){ $.extend(config, options); }
@@ -19,9 +19,6 @@ $.fn.fixedHeader = function (options) {
    if (!o.is(':visible')) return;
    if ($('thead.header-copy').size()) {
      $('thead.header-copy').width($head.width());
-      $('thead.header-copy th').each(function(i, th) {
-        $(th).width($($head.find('th')[i]).width());
-      });
      var i, scrollTop = $win.scrollTop();
    }
    var t = $head.length && $head.offset().top - config.topOffset;
@@ -33,8 +30,21 @@ $.fn.fixedHeader = function (options) {
    }
    isFixed ? $('thead.header-copy', o).show().offset({ left: $head.offset().left })
            : $('thead.header-copy', o).hide();
+    // NG: dislocate while iframe page resized. fixed by jeffen@pactera 2015/7/8
+	headerCopyRectify();
  }
+
+  // set a broken bone when header copy dislocated
+  function headerCopyRectify() {
+    o.find('thead.header > tr > th').each(function (i, h) {
+      var w = $(h).width();
+      o.find('thead.header-copy> tr > th:eq('+i+')').width(w)
+    });
+  }
+
  $win.on('scroll', processScroll);
+  // NG: dislocate while body resized. fixed by jeffen@pactera 2015/7/9
+  $win.on('resize', processScroll);

  // hack sad times - holdover until rewrite for 2.1
  $head.on('click', function () {
@@ -44,14 +54,13 @@ $.fn.fixedHeader = function (options) {
  $head.clone(true).removeClass('header').addClass('header-copy header-fixed').css({'position': 'fixed', 'top': config['topOffset']}).appendTo(o);
  o.find('thead.header-copy').width($head.width());

-  o.find('thead.header > tr > th').each(function (i, h) {
-    var w = $(h).width();
-    o.find('thead.header-copy> tr > th:eq('+i+')').width(w)
-  });
+  headerCopyRectify();
  $head.css({ margin:'0 auto',
              width: o.width(),
             'background-color':config.bgColor });
  processScroll();
+
+
 });
};
