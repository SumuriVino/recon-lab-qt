#ifndef __QSLICE_H
#define __QSLICE_H

#include <QGLWidget>

class QMouseEvent ;

class QSlice : public QGLWidget
{
	Q_OBJECT
	
	private:
		double		_currentMin;	
		double		_currentMax;	
		double		_valueMin;		
		double		_valueMax;		
		double		_cursorSize;	
		double 		_mouseX;		
		int 		_moved;			
		bool 		_onMin;			
		bool 		_onMax;			
	
	public:
		QSlice(QWidget *parent = NULL);	
		~QSlice();						
		
		QSize sizeHint() const;			
		
		//getters
		int currentMin();
		int currentMax();
		int valueMin();
		int valueMax();
		
		//setters
		void setValueMin(int valueMin);
		void setValueMax(int valueMax);
	
	public slots:
		void setCurrentMin(int currentMin);
		void setCurrentMax(int currentMax);
	
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);
		void paintEvent(QPaintEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
	
		void drawCursor(double pos, int id);
											

	protected slots:
		void printVals(int = 0);
		
	signals:
		void minChanged(int);	
		void maxChanged(int);	
};

#endif
