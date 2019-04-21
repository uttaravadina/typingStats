#include "textentrywidget.h"
#include "textentryovertypewidget.h"

#include "keyevent.h"
#include "storagemanager.h"
#include <QDebug>
#include <QEvent>
#include <QLayout>
#include <QPushButton>
#include <QTimer>

TextEntryWidget::TextEntryWidget( QWidget *parent ) : QWidget( parent ) {
	createDisplayWindow();
	createButtons();

	m_countDownBar = new QProgressBar;
	m_countDownBar->setRange( 0, 100 );

	setupLayout();
	setupConnections();
}
TextEntryWidget::~TextEntryWidget() {}

void TextEntryWidget::createDisplayWindow() {
	m_textEntryWidget = new TextEntryOverTypeWidget();
}

void TextEntryWidget::createButtons() {
	m_buttonBox = new QHBoxLayout;
	QPushButton *pauseButton = new QPushButton( "pause" );
	QPushButton *cancelButton = new QPushButton( "cancel" );

	m_buttonBox->addWidget( pauseButton );
	m_buttonBox->addWidget( cancelButton );

	connect( cancelButton, &QPushButton::released, this,
			 &TextEntryWidget::close );
}

void TextEntryWidget::setupLayout() {
	QVBoxLayout *lay = new QVBoxLayout;
	lay->addWidget( m_textEntryWidget );
	lay->addWidget( m_countDownBar );
	lay->addLayout( m_buttonBox );
	setLayout( lay );
}

void TextEntryWidget::setupConnections() {
	connect( m_textEntryWidget, &AbstractTextEntryWidget::timePercentLeftUpdate,
			 m_countDownBar, &QProgressBar::setValue );
	connect( m_textEntryWidget, &AbstractTextEntryWidget::testFinished, this,
			 &TextEntryWidget::testHasEnded );
}

void TextEntryWidget::startTest( QString text, double durationMsec ) {
	// TODO fix the time to uint seconds.
	m_textEntryWidget->setTest( text, durationMsec / 1000 );
}

void TextEntryWidget::testHasEnded( TestResult result ) {
	emit testFinished( result );
}

void TextEntryWidget::endTest() {
	TestResult result = m_textEntryWidget->endTest();
	emit testFinished( result );
}
