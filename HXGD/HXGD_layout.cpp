#include "HXGD.h"



void HXGD::InitUILayout()
{
	//表格设置自适应宽度
	ui.tableWidget_layoutInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_layoutInfo->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置选择行为时每次选择一行
	ui.tableWidget_layoutInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可编辑
	ui.tableWidget_layoutInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置选中背景色
	ui.tableWidget_layoutInfo->setStyleSheet("selection-background-color:lightblue;");
	//设置表头背景色
	ui.tableWidget_layoutInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	ui.tableWidget_layoutInfo->setContextMenuPolicy(Qt::CustomContextMenu);


	//表格设置自适应宽度
	ui.tableWidget_panelinfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_panelinfo->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置选择行为时每次选择一行
	ui.tableWidget_panelinfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可编辑
	//ui.Table_GroupAlgParaList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置选中背景色
	ui.tableWidget_panelinfo->setStyleSheet("selection-background-color:lightblue;");
	//设置表头背景色
	ui.tableWidget_panelinfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	ui.tableWidget_panelinfo->setContextMenuPolicy(Qt::CustomContextMenu);

	ui.tableWidget_layoutInfo->setMouseTracking(true);//开启捕获鼠标功能

	connect(ui.tableWidget_layoutInfo, SIGNAL(entered(QModelIndex)), this, SLOT(slotShowTooltip(QModelIndex)));



	connect(ui.tableWidget_layoutInfo, &QTableWidget::currentCellChanged, this, &HXGD::slotLayoutCellChanged);
	connect(ui.tableWidget_panelinfo, &QTableWidget::currentCellChanged, this, &HXGD::slotLayoutSmallPanelCellChanged);
	
}


void HXGD::LoadAllLayoutData()
{
	//加载layout信息
	//QString path = QString("%1/ConfigureXML/PanelLayout.xml").arg(QApplication::applicationDirPath());
	QFile file(config_info.layout_xml);
	bool b = file.open(QIODevice::ReadOnly);
	if (!b) return;

	QDomDocument doc;
	b = doc.setContent(&file);
	if (!b) return;
	file.close();

	QDomElement root = doc.documentElement();//返回根节点元素
	QString root_node_name = root.nodeName();
	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName() == "LayoutList")
			{
				int row_count = ui.tableWidget_layoutInfo->rowCount();
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Layout")
					{
						//QString s = node2.toElement().attribute("layout_name");
						CurrentLayoutInfo currentLayoutInfo;
						currentLayoutInfo.layoutName = node2.toElement().attribute("name");
						/*currentLayoutInfo.productID = node2.toElement().attribute("product_id");
						currentLayoutInfo.productSize = node2.toElement().attribute("product_size");*/
						currentLayoutInfo.updateTime = node2.toElement().attribute("updateTime");
						switch (node2.toElement().attribute("startpos").toInt())
						{
						case 0:
							currentLayoutInfo.layoutDirection = node2.toElement().attribute("右下角");
							break;
						case 1:
							currentLayoutInfo.layoutDirection = node2.toElement().attribute("右上角");
							break;

						case 2:
							currentLayoutInfo.layoutDirection = node2.toElement().attribute("左上角");
							break;
						case 3:
							currentLayoutInfo.layoutDirection = node2.toElement().attribute("左下角");
							break;
						default:
							break;
						}
						currentLayoutInfo.remarks = node2.toElement().attribute("remark");
						ui.tableWidget_layoutInfo->setRowCount(row_count+1);
						ui.tableWidget_layoutInfo->setItem(row_count, 0, new QTableWidgetItem(currentLayoutInfo.layoutName));
						/*ui.tableWidget_layoutInfo->setItem(row_count, 1, new QTableWidgetItem(currentLayoutInfo.productID));
						ui.tableWidget_layoutInfo->setItem(row_count, 2, new QTableWidgetItem(currentLayoutInfo.productSize));*/
						ui.tableWidget_layoutInfo->setItem(row_count, 3, new QTableWidgetItem(currentLayoutInfo.updateTime));
						ui.tableWidget_layoutInfo->setItem(row_count, 4, new QTableWidgetItem(currentLayoutInfo.layoutDirection));
						ui.tableWidget_layoutInfo->setItem(row_count, 5, new QTableWidgetItem(currentLayoutInfo.remarks));

						//追加recipe编辑界面中的布局combox的内容
						ui.cbx_recipeLayout->addItem(node2.toElement().attribute("name"));
						//ui.cbx_recipeProductSize->addItem(node2.toElement().attribute("product_size"));
					}
					node2 = node2.nextSibling();
					++row_count;
				}
			}
			node1 = node1.nextSibling();
		}
	}
}

void HXGD::LoadPanelDataByProductID(QString layoutname,QList<SmallPanel>& templist_panel_Info)
{
	QFile file(config_info.layout_xml);
	bool b = file.open(QIODevice::ReadOnly);
	if (!b)
	{
		return;
	}
	QDomDocument doc;
	b = doc.setContent(&file);
	if (!b)
	{
		return;
	}
	file.close();
	QDomElement root = doc.documentElement();
	QString root_node_name = root.nodeName();
	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName() == "LayoutList")
			{
				QDomNode node2 = node1.firstChild();
				ui.widget_layout->clearPanelInfoList();

				while (!node2.isNull())
				{
					if (node2.nodeName() == "Layout")
					{
						if (node2.toElement().attribute("name") == layoutname)
						{
							ui.widget_layout->setXValue(node2.toElement().attribute("width").toDouble());
							ui.widget_layout->setYValue(node2.toElement().attribute("length").toDouble());
							ui.widget_layout->setStartPos(node2.toElement().attribute("startpos").toInt());


							QDomNode node3 = node2.firstChild();
							while (!node3.isNull())
							{
								SmallPanel m_panelInfo;
								
								m_panelInfo.layoutname = node3.toElement().attribute("id");
								m_panelInfo.x = node3.toElement().attribute("x");
								m_panelInfo.y = node3.toElement().attribute("y");
								m_panelInfo.width = node3.toElement().attribute("width");
								m_panelInfo.height = node3.toElement().attribute("length");
							/*	m_panelInfo.left_margin = node3.toElement().attribute("L_M");
								m_panelInfo.right_margin = node3.toElement().attribute("R_M");
								m_panelInfo.top_margin = node3.toElement().attribute("T_M");
								m_panelInfo.bottom_margin = node3.toElement().attribute("B_M");*/
								m_panelInfo.remarks = node3.toElement().attribute("remarks");
								
								templist_panel_Info.append(m_panelInfo);
								LoadSPanelInfo(ui.widget_layout, node3);
								//ui.widget_layout->addPanelInfoList(s_panelInfo);
								node3 = node3.nextSibling();
							}
						}
						node2 = node2.nextSibling();
					}
				}
			}
			node1 = node1.nextSibling();
		}
	}

}

void HXGD::ExpandSmallPanel(QTableWidget* tablewidget, QList<SmallPanel> list_panle)
{
	tablewidget->clear();
	for (int i = 0; i < list_panle.size(); ++i)
	{
		tablewidget->setRowCount(i + 1);
		tablewidget->setItem(i, 0, new QTableWidgetItem(list_panle.at(i).layoutname));
		tablewidget->setItem(i, 1, new QTableWidgetItem(list_panle.at(i).x));
		tablewidget->setItem(i, 2, new QTableWidgetItem(list_panle.at(i).y));
		tablewidget->setItem(i, 3, new QTableWidgetItem(list_panle.at(i).width));
		tablewidget->setItem(i, 4, new QTableWidgetItem(list_panle.at(i).height));
	/*	tablewidget->setItem(i, 5, new QTableWidgetItem(list_panle.at(i).left_margin));
		tablewidget->setItem(i, 6, new QTableWidgetItem(list_panle.at(i).right_margin));
		tablewidget->setItem(i, 7, new QTableWidgetItem(list_panle.at(i).top_margin));
		tablewidget->setItem(i, 8, new QTableWidgetItem(list_panle.at(i).bottom_margin));*/
		tablewidget->setItem(i, 9, new QTableWidgetItem(list_panle.at(i).remarks));
	}
}

void HXGD::ShowCurrentLayoutSettings(CurrentLayoutInfo currentLayoutInfo)
{
	ui.LE_layoutLayoutName->setText(currentLayoutInfo.layoutName);
	ui.LE_layoutProductID->setText(currentLayoutInfo.productID);
	ui.LE_layoutProductSize->setText(currentLayoutInfo.productSize);
	ui.LE_layoutUpdatetime->setText(currentLayoutInfo.updateTime);
	switch (currentLayoutInfo.layoutDirection.toInt())
	{
	case StartOrPOS::START_LT:
		ui.CB_layoutDirection->setCurrentIndex(0);
		break;
	case StartOrPOS::START_RT:
		ui.CB_layoutDirection->setCurrentIndex(1);
		break;
	case StartOrPOS::START_RD:
		ui.CB_layoutDirection->setCurrentIndex(2);
		break;
	case StartOrPOS::START_LD:
		ui.CB_layoutDirection->setCurrentIndex(3);
		break;
	default:
		break;
	}
	ui.LE_layoutRemarks->setText(currentLayoutInfo.remarks);
	
}

void HXGD::LoadSPanelInfo( LayoutBar* layoutbar, QDomNode node)
{
	PanelInfo s_panelInfo;
	s_panelInfo.x = node.toElement().attribute("x").toDouble();
	s_panelInfo.y = node.toElement().attribute("y").toDouble();
	s_panelInfo.width = node.toElement().attribute("width").toDouble();
	s_panelInfo.height = node.toElement().attribute("length").toDouble();
	s_panelInfo.str = node.toElement().attribute("id");
	layoutbar->addPanelInfoList(s_panelInfo);
}

void HXGD::LayoutLoadByNameAndWidget(QString str, LayoutBar& bar, LayoutPara& Lp, bool bLoadRecipe)
{

	//Recipe下参数读取
	QFile file(config_info.layout_xml);
   //只读方式打开文件
	bool isOK = file.open(QIODevice::ReadOnly);

	//文件file和xml关联
	QDomDocument doc;
	isOK = doc.setContent(&file);
	if (false == isOK) {

	}
	//关联成功后，可以关闭文件
	file.close();
	//获取根节点
	QDomElement root = doc.documentElement();
	if (root.hasChildNodes()) {  //有没有子节点
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName() == "LayoutList")
			{
				QDomNode node2 = node1.firstChild();
				int i = 0;
				ui.widget_layout->clearPanelInfoList();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Layout" && node2.toElement().attribute("name") == str)
					{
						if (bLoadRecipe)
						{
							currentRecipe.layout.listPanel.clear();
						}
						Lp.listPanel.clear();
						bar.setXValue(node2.toElement().attribute("width").toDouble());
						if (bLoadRecipe)
						{
							currentRecipe.layout.width = node2.toElement().attribute("width").toDouble();
						}
						Lp.width = node2.toElement().attribute("width").toDouble();
						//bar.setXMinValue(-node2.toElement().attribute("width").toDouble() / 2);

						bar.setYValue(node2.toElement().attribute("length").toDouble());
						if (bLoadRecipe)
						{
							currentRecipe.layout.height = node2.toElement().attribute("length").toDouble();
						}
						Lp.height = node2.toElement().attribute("length").toDouble();

						bar.setStartPos(node2.toElement().attribute("startpos").toInt());

						//bar.setYMinValue(-node2.toElement().attribute("length").toDouble() / 2);
						QDomNode node3 = node2.firstChild();
						int j = 0;
						while (!node3.isNull())
						{
							PanelInfo pl;
							pl.x = node3.toElement().attribute("x").toDouble();
							pl.y = node3.toElement().attribute("y").toDouble();
							pl.width = node3.toElement().attribute("width").toDouble();
							pl.height = node3.toElement().attribute("length").toDouble();
							pl.str = node3.toElement().attribute("id");
							bar.addPanelInfoList(pl);

							cv::Rect2f panelRect(pl.x, pl.y, pl.width, pl.height);
							PanelPara pp;
							pp.panelRect = panelRect;
							pp.panelNum = node3.toElement().attribute("num").toInt();
							if (pp.panelNum == 0)
							{
								pp.panelNum = 1;
							}
							pp.panelID = node3.toElement().attribute("id");
							if (bLoadRecipe)
							{
								currentRecipe.layout.listPanel.push_back(pp);
							}

							Lp.listPanel.push_back(pp);

							i++;
							node3 = node3.nextSibling();
						}
					}
					node2 = node2.nextSibling();
				}
			}
			node1 = node1.nextSibling();
		}
	}
	else {  //没有子节点
		return;
	}
	bar.repaint();
}

void HXGD::slotShowTooltip(QModelIndex index)
{
	QToolTip::showText(QCursor::pos(), index.data().toString());
	return;
}

void HXGD::slotLayoutSmallPanelCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
	currentRowIndex = currentRow;
	SmallPanel temp_smallpanel;
	temp_smallpanel.layoutname = ui.tableWidget_panelinfo->item(currentRowIndex, 0)->text();
	temp_smallpanel.x = ui.tableWidget_panelinfo->item(currentRowIndex, 1)->text();
	temp_smallpanel.y = ui.tableWidget_panelinfo->item(currentRowIndex, 2)->text();
	temp_smallpanel.width = ui.tableWidget_panelinfo->item(currentRowIndex, 3)->text();
	temp_smallpanel.height = ui.tableWidget_panelinfo->item(currentRowIndex, 4)->text();
	/*temp_smallpanel.left_margin = ui.tableWidget_panelinfo->item(currentRowIndex, 5)->text();
	temp_smallpanel.right_margin = ui.tableWidget_panelinfo->item(currentRowIndex, 6)->text();
	temp_smallpanel.top_margin = ui.tableWidget_panelinfo->item(currentRowIndex, 7)->text();
	temp_smallpanel.bottom_margin = ui.tableWidget_panelinfo->item(currentRowIndex, 8)->text();*/
	temp_smallpanel.remarks = ui.tableWidget_panelinfo->item(currentRowIndex, 9)->text();
	ui.layout_smallpanel->SetPanelInfo(temp_smallpanel.width.toDouble(), temp_smallpanel.height.toDouble());
}

void HXGD::on_tableWidget_layoutInfo_customContextMenuRequested(const QPoint& pos)
{
	table_widget_menu = new QMenu(ui.tableWidget_layoutInfo);
	QAction* action = new QAction(tr("模板导入"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(slotActionImportTemplate()));
	table_widget_menu->addAction(action);

	action = new QAction(tr("新增"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(slotActionAddLayout()));
	table_widget_menu->addAction(action);

	action = new QAction(tr("刷新"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(slotActionRefresh()));
	table_widget_menu->addAction(action);
	table_widget_menu->exec(QCursor::pos());
}

void HXGD::slotActionImportTemplate()
{


}
/// <summary>
/// 新增Layout以最后一行的layout为模板进行新增
/// </summary>
void HXGD::slotActionAddLayout()
{
	//1、获取最后一行的Product ID
	//2、根据Product ID去复制相关的信息
	QString currentProductID = ui.tableWidget_layoutInfo->item(currentRowIndex, 1)->text();
	int index = 1;
	QString tempName = QString("新建布局%1").arg(index);
	while (true)
	{
		bool bSameName = false;
		for (size_t i = 0; i < ui.tableWidget_layoutInfo->rowCount(); i++)
		{
			if (tempName == ui.tableWidget_layoutInfo->item(i, 1)->text())
			{
				bSameName = true;
			}
		}
		if (!bSameName)
		{
			break;
		}
		else {
			index += 1;
			tempName = QString("新建布局%1").arg(index);
		}
	};
	QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	ui.tableWidget_layoutInfo->setRowCount(ui.tableWidget_layoutInfo->rowCount() + 1);
	ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount()-1, 0, new QTableWidgetItem(tempName));
	ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount()-1, 3, new QTableWidgetItem(dateTime));
	QFile file(config_info.layout_xml);
	bool b = file.open(QIODevice::ReadOnly|QFile::Text);
	if (!b)
	{
		return;
	}
	QDomDocument doc;
	b = doc.setContent(&file);
	if (!b)
	{
		return;
	}
	file.close();
	QDomElement root = doc.documentElement();
	QString root_node_name = root.nodeName();
	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		QDomNode copyNode;
		while (!node1.isNull())
		{
			if (node1.nodeName()=="LayoutList")
			{
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Layout" && node2.toElement().attribute("name") == currentProductID)
					{
						copyNode = node2.cloneNode();
						copyNode.toElement().setAttribute("layout_name", tempName);
						copyNode.toElement().setAttribute("update_time", dateTime);
						ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount() - 1, 1, new QTableWidgetItem(node2.toElement().attribute("product_id")));
						ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount() - 1, 2, new QTableWidgetItem(node2.toElement().attribute("product_size")));
						ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount() - 1, 4, new QTableWidgetItem(node2.toElement().attribute("layout_direction")));
						ui.tableWidget_layoutInfo->setItem(ui.tableWidget_layoutInfo->rowCount() - 1, 5, new QTableWidgetItem(node2.toElement().attribute("remarks")));
					}
					node2 = node2.nextSibling();
				}
				node1.appendChild(copyNode);
			}
			node1 = node1.nextSibling();
		}
	}
	b = file.open(QIODevice::WriteOnly | QFile::Truncate);
	QTextStream stream(&file);
	stream.reset();
	stream.setCodec("utf-8");
	doc.save(stream, 4, QDomNode::EncodingFromTextStream);
	file.close();

}

void HXGD::slotActionRefresh()
{


}





void HXGD::slotLayoutCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
	currentRowIndex = currentRow;
	QString layoutname = ui.tableWidget_layoutInfo->item(currentRowIndex, 0)->text();//行、列分别从0、1开始计数
	CurrentLayoutInfo currentLayoutInfo;
	currentLayoutInfo.layoutName = ui.tableWidget_layoutInfo->item(currentRowIndex, 0)->text();
	/*currentLayoutInfo.productID = ui.tableWidget_layoutInfo->item(currentRowIndex, 1)->text();
	currentLayoutInfo.productSize = ui.tableWidget_layoutInfo->item(currentRowIndex, 2)->text();*/
	currentLayoutInfo.updateTime = ui.tableWidget_layoutInfo->item(currentRowIndex, 3)->text();
	currentLayoutInfo.layoutDirection = ui.tableWidget_layoutInfo->item(currentRowIndex, 4)->text();
	currentLayoutInfo.remarks = ui.tableWidget_layoutInfo->item(currentRowIndex, 5)->text();

	//list_panelinfo.clear();
	QList<SmallPanel> list_panelinfo;
	LoadPanelDataByProductID(layoutname, list_panelinfo);
	ExpandSmallPanel(ui.tableWidget_panelinfo, list_panelinfo);
	ShowCurrentLayoutSettings(currentLayoutInfo);
}
