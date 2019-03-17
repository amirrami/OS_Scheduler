/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>

View::View(QWidget *parent,QVector<QString> signal,QVector<QString> signalNames,int NumberOCycles)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

/****************************************************************************/
    // chart setup
    m_chart = new QChart;
    m_chart->setMinimumSize(640, 480);
    m_chart->setTitle("Hover the line to show callout. Click the line to make it stay");
    m_chart->legend()->hide();
    // axis setup
    QValueAxis *axisX = new QValueAxis;
    QCategoryAxis *axisY = new QCategoryAxis();
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_chart->setTitle("PCI simulatiom");
    axisX->setRange(0,NumberOCycles*2);
    axisX->setTickCount(NumberOCycles*2+1);
    axisY->setRange(0,30);
    axisY->setTickCount(31);
    axisX->setLabelsVisible(true);
    axisY->setLabelsVisible(true);


    /****************************************************************************/

    SignalValues.resize(signal.size());
    SignalValuesRange.resize(signal.size());
    int dataError=0;
    for(int i =0; i<signal.size();i++)
    {
        if(signalNames[i]=="AD" || signalNames[i]=="CBE")
        {
            SignalValues.resize(signal.size()-1);
            SignalValuesRange.resize(signal.size()-1);

            QPen pen(QRgb(0xfdb157));
            pen.setWidth(3);
            View::extrctData(signal[i+1],(i-dataError));
            QString baseSignal = View::NotBit(signal[i]);
            QString mirrorSignal=View::NotBitMirror(signal[i]);
            NumberofPoints(baseSignal);
            QLineSeries *series = View::GetPoints(baseSignal,(i-dataError)*2,(i-dataError),false,false);
            series->setPen(pen);
            m_chart->addSeries(series);
            connect(series, &QLineSeries::clicked, this, &View::keepCallout);
            connect(series, &QLineSeries::hovered, this, &View::tooltip);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
            series = View::GetPoints(mirrorSignal,(i-dataError)*2,(i-dataError),true,false);
            series->setPen(pen);
            m_chart->addSeries(series);
            axisY->append(signalNames[i],((i-dataError)+1)*2);
            connect(series, &QLineSeries::clicked, this, &View::keepCallout);
            connect(series, &QLineSeries::hovered, this, &View::tooltip);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
            i++;
            dataError++;
            SignalValues.resize(signal.size()-1);
            SignalValuesRange.resize(signal.size()-1);
        }
        else{
        QLineSeries *series = View::GetPoints(signal[i],(i-dataError)*2,i-dataError,false,true);
        m_chart->addSeries(series);
        axisY->append(signalNames[i],((i-dataError)+1)*2);
        connect(series, &QLineSeries::clicked, this, &View::keepCallout);
        connect(series, &QLineSeries::hovered, this, &View::tooltip);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
        }
    }

    /****************************************************************************/

    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");


    this->setMouseTracking(true);
}


/****************************************************************************/


void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
         const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}


/****************************************************************************/


void View::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}


/****************************************************************************/



void View::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}


/****************************************************************************/



void View::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        for(int j=0;j<SignalValuesRange.size();j++){
         for(int i=0;i<SignalValuesRange[j].size();i++)
        {
            if(i==0)
            {
                if(point.x()>0 && point.x()<SignalValuesRange[j][i] && point.y()>((float)(j*2)-0.2) && point.y()<(j+1)*2)
                m_tooltip->setText(SignalValues[j][i]);
            }
            else
            {
                if(point.x()>SignalValuesRange[j][i-1] && point.x()<SignalValuesRange[j][i]  && point.y()>((float)(j*2)-0.2) && point.y()<(j+1)*2 )
                     m_tooltip->setText(SignalValues[j][i]);
            }
        }
        }
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}


/****************************************************************************/


//my functions
int View::NumberofPoints(QString signal){
    int NumOfPoints =0;
    if(signal=="")
        return 0;
    QString bit="";
    for(int i=0;i<signal.size()-1;i++){
        bit=signal[i];
        if(signal[i]==signal[i+1])
            NumOfPoints+=1;
        else if(bit=="/")
            NumOfPoints+=3;
        else
            NumOfPoints+=2;
    }
    return NumOfPoints;
}


/****************************************************************************/



QLineSeries* View::GetPoints(QString signal,int Yindex,int SignalIndex,bool mirror,bool bitCheck)
{

    int signalSize =signal.size();
    QLineSeries *series = new QLineSeries;
    QString bit="";
    QString bit_1="";
    QString bit_2="";
    QPointF point;
    QVector<QPointF> points;
    int slashError=0;
    for(int i=0;i<signalSize;i++){
         bit=signal[i];
         if(i!=0)
            bit_1=signal[i-1];
         if(i>1)
            bit_2=signal[i-2];
         else if(i==0)
             bit_1=bit;
         point.setX(i-slashError);
         if(bit==bit_1 && i!=0 )
         {
            if(bit=="x")
            {
                point.setY(0.5+Yindex);
                points.append(point);
            }
            else
            {
                point.setY(bit.toInt()+Yindex);
                points.append(point);
            }
         }
         else if(bit!=bit_1 && i!=0 && bit!="/"&& bit_1!="/")
         {
             if(bit_1=="x")
             {
                 point.setY(0.5+Yindex);
                 points.append(point);
                 if(!mirror)
                 {
                     SignalValuesRange[SignalIndex].append(i-slashError);
                     SignalValues[SignalIndex].append(bit);
                 }
             }
             else
             {
                 point.setY(bit_1.toInt()+Yindex);
                 points.append(point);
                 if(!mirror)
                 {
                     SignalValuesRange[SignalIndex].append(i-slashError);
                     SignalValues[SignalIndex].append(bit);
                 }
             }
             point.setX(i-slashError);
             if(bit=="x")
             {
                 point.setY(0.5+Yindex);
                 points.append(point);
             }
             else
             {
                 if(bit=="1"){
                     point.setY(1+Yindex);
                     points.append(point);
                 }
                 else
                 {
                     point.setY(0+Yindex);
                     points.append(point);
                 }

             }
         }
         else if(bit=="/")
         {

             if(bit_1=="1")
             {
                 point.setY(1+Yindex);
                 points.append(point);
                 point.setX(i-slashError);
                 point.setY(0+Yindex);
                 points.append(point);
                 point.setX(i-slashError);
                 point.setY(1+Yindex);
                 points.append(point);
                 if(!mirror)
                    SignalValuesRange[SignalIndex].append(i-slashError);
             }
             else if(bit_1=="0")
             {
                 point.setY(0+Yindex);
                 points.append(point);
                 point.setX(i-slashError);
                 point.setY(1+Yindex);
                 points.append(point);
                 point.setX(i-slashError);
                 point.setY(0+Yindex);
                 points.append(point);
                 if(!mirror)
                    SignalValuesRange[SignalIndex].append(i-slashError);
             }
         }
         else if(bit_1=="/")
         {
             slashError++;
             continue;
         }
         else
         {
             if(bit=="x")
             {
                 point.setY(0.5+Yindex);
                 points.append(point);
                 if(bitCheck && !mirror)
                    SignalValues[SignalIndex].append("x");
             }
             else
             {
                 point.setY(bit.toInt()+Yindex);
                 points.append(point);
                 if(bitCheck && !mirror)
                    SignalValues[SignalIndex].append(bit);
             }
         }
    }

    if(points.size()>0){
        point.setX(points.last().x()+1);
        point.setY(points.last().y());
        SignalValuesRange[SignalIndex].append(points.last().x()+1);
        points.append(point);
    }


    for(int i=0;i<points.size();i++){
        *series << points[i];
    }
 return series;
}

/****************************************************************************/


QString View::NotBit(QString signal){

    QString bit="";
    QString mirrorString="";
    for(int i=0;i<signal.size();i++){
        bit=signal[i];
       if(bit=="x")
       {
           mirrorString+="1";
       }
       else if(bit=="1")
       {
           mirrorString+="0";
       }
       else if(bit=="/")
       {
           mirrorString+="/";
       }
    }
    return mirrorString;
}
QString View::NotBitMirror(QString signal){

    QString bit="";
    QString mirrorString="";
    for(int i=0;i<signal.size();i++){
        bit=signal[i];
       if(bit=="x")
       {
           mirrorString+="0";

       }
       else if(bit=="1")
       {
           mirrorString+="1";
       }
       else if(bit=="/")
       {
           mirrorString+="/";
       }
    }
    return mirrorString;
}


/****************************************************************************/


void View::extrctData(QString signalData,int SignalIndex){
    QString data="";
    QString bit="";
    for(int i=0; i<signalData.size();i++){
        bit=signalData[i];
         if(bit=="/")
         {
            SignalValues[SignalIndex].append(data);
            data="";
         }
         else
         {
            data+=bit;
         }
    }
   SignalValues[SignalIndex].append(data);
}
