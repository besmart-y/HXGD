#include "HXGD.h"


void HXGD::InitUIRecipe()
{
	//表格设置自适应宽度
	ui.tableWidget_RecipeIDInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_RecipeIDInfo->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置选择行为时每次选择一行
	ui.tableWidget_RecipeIDInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可编辑
	ui.tableWidget_RecipeIDInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置选中背景色
	ui.tableWidget_RecipeIDInfo->setStyleSheet("selection-background-color:lightblue;");
	//设置表头背景色
	ui.tableWidget_RecipeIDInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	ui.tableWidget_RecipeIDInfo->setContextMenuPolicy(Qt::CustomContextMenu);
	

	////表格设置自适应宽度
	//ui.tableWidget_CurrentRecipeInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget_CurrentRecipeInfo->setSelectionMode(QAbstractItemView::ExtendedSelection);
	////设置选择行为时每次选择一行
	//ui.tableWidget_CurrentRecipeInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	////设置不可编辑
	////ui.Table_GroupAlgParaList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	////设置选中背景色
	//ui.tableWidget_CurrentRecipeInfo->setStyleSheet("selection-background-color:lightblue;");
	////设置表头背景色
	//ui.tableWidget_CurrentRecipeInfo->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	//ui.tableWidget_CurrentRecipeInfo->setContextMenuPolicy(Qt::CustomContextMenu);

	//ui.tableWidget_CurrentRecipeInfo->setMouseTracking(true);//开启捕获鼠标功能

	//	//表格设置自适应宽度
	//ui.tableWidget_CurrentRecipeAlg->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//ui.tableWidget_CurrentRecipeAlg->setSelectionMode(QAbstractItemView::ExtendedSelection);
	////设置选择行为时每次选择一行
	//ui.tableWidget_CurrentRecipeAlg->setSelectionBehavior(QAbstractItemView::SelectRows);
	////设置不可编辑
	////ui.Table_GroupAlgParaList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	////设置选中背景色
	//ui.tableWidget_CurrentRecipeAlg->setStyleSheet("selection-background-color:lightblue;");
	////设置表头背景色
	//ui.tableWidget_CurrentRecipeAlg->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	//ui.tableWidget_CurrentRecipeAlg->setContextMenuPolicy(Qt::CustomContextMenu);

	//ui.tableWidget_CurrentRecipeAlg->setMouseTracking(true);//开启捕获鼠标功能

	//表格设置自适应宽度
	ui.tableWidget_RecipeRepos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_RecipeRepos->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//设置选择行为时每次选择一行
	ui.tableWidget_RecipeRepos->setSelectionBehavior(QAbstractItemView::SelectRows);
	//设置不可编辑
	ui.tableWidget_RecipeRepos->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//设置选中背景色
	ui.tableWidget_RecipeRepos->setStyleSheet("selection-background-color:lightblue;");
	//设置表头背景色
	ui.tableWidget_RecipeRepos->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

	ui.tableWidget_RecipeRepos->setContextMenuPolicy(Qt::CustomContextMenu);

	ui.tableWidget_RecipeRepos->setMouseTracking(true);//开启捕获鼠标功能

	connect(ui.tableWidget_layoutInfo, SIGNAL(entered(QModelIndex)), this, SLOT(slotShowTooltip(QModelIndex)));



	connect(ui.tableWidget_RecipeIDInfo, &QTableWidget::currentCellChanged, this, &HXGD::slotRecipeIDCellChanged);
	//connect(ui.tableWidget_CurrentRecipeInfo, &QTableWidget::currentCellChanged, this, &HXGD::slotCurrentRecipeInfoCellChanged);
	connect(ui.tableWidget_RecipeRepos, &QTableWidget::currentCellChanged, this, &HXGD::slotCurrentRecipeReposCellChanged);
}

void HXGD::LoadAllRecipes()
{
	//加载时先清空原来的combox数据
	ui.cbx_recipeID->clear();

	QFile file(config_info.receipe_xml);
	bool b = file.open(QIODevice::ReadOnly);
	if (!b) return;
	QDomDocument doc;
	b = doc.setContent(&file);
	if (!b) return;
	file.close();
	QDomElement root = doc.documentElement();
	QString root_node_name = root.nodeName();
	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName()=="RecipeList")
			{
				int rowCount = 0;
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe")
					{
					
						ui.tableWidget_RecipeRepos->setRowCount(rowCount + 1);
						QString s = node2.toElement().attribute("name");
						ui.tableWidget_RecipeRepos->setItem(rowCount, 0, new QTableWidgetItem(s));
			
						
						/*ui.tableWidget_RecipeRepos->setItem(rowCount, 2, new QTableWidgetItem(node2.toElement().attribute("product_layout")));*/
						ui.tableWidget_RecipeRepos->setItem(rowCount,2, new QTableWidgetItem(node2.toElement().attribute("recipeID")));
						ui.tableWidget_RecipeRepos->setItem(rowCount,3, new QTableWidgetItem(node2.toElement().attribute("updateTime")));
						ui.tableWidget_RecipeRepos->setItem(rowCount, 4, new QTableWidgetItem(node2.toElement().attribute("remark")));

						QDomNode node3 = node2.firstChild();
						while (!node3.isNull())
						{
							if (node3.nodeName()=="BaseRecipe" && node3.toElement().attribute("plugname")=="GLassLayout")
							{
								ui.tableWidget_RecipeRepos->setItem(rowCount, 1, new QTableWidgetItem(node3.toElement().attribute("name")));
							}
							node3 = node3.nextSibling();
						}
		

					}
					++rowCount;
					node2 = node2.nextSibling();
				}
			
			}
			if (node1.nodeName()=="RecipeIDBinding")
			{
				int rowCount = 0; 
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "RecipeID")
					{
						ui.tableWidget_RecipeIDInfo->setRowCount(rowCount + 1);
						ui.tableWidget_RecipeIDInfo->setItem(rowCount, 0, new QTableWidgetItem(node2.toElement().attribute("recipe_id")));
						ui.tableWidget_RecipeIDInfo->setItem(rowCount, 1, new QTableWidgetItem(node2.toElement().attribute("bind_recipe")));
						
						ui.cbx_recipeID->addItem(node2.toElement().attribute("recipe_id"));
					}
					++rowCount;
					node2 = node2.nextSibling();
				}
			}

			node1 = node1.nextSibling();
		}
	}
}

//注：此方法要求配方库库中不能有布局名称和产品尺寸名称相同的配方
void HXGD::ShowCurrentRecipeInfo(QString currentRecipeName, QString currentSize,QString currentLayout)
{
	int panelCount = 0;
	QList<QMap<QString, QString>> listPanelsAlg;
	QMap<QString, QString> panelAlg;

	QFile file(config_info.receipe_xml);
	bool b = file.open(QIODevice::ReadOnly);
	if (!b) return;
	QDomDocument doc;
	b = doc.setContent(&file);
	if (!b) return;
	file.close();
	QDomElement root = doc.documentElement();

	QString root_node_name = root.nodeName();

	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if(node1.nodeName()=="RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				int rowCount = 0;
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe"&&node2.toElement().attribute("name") == currentRecipeName)
					{

						QStringList lables;
						lables << tr("配方名称") << tr("产品尺寸") << tr("布局名称") << tr("更新时间") << tr("备注");
						//lables << "配方名称" << "产品尺寸" << "布局名称" << "更新时间" << "备注";
						//lables << "s1" << "s2" << "s3" << "s4" << "s5";

						ui.LE_recipeName->setText(node2.toElement().attribute("name"));
						//ui.cbx_recipeProductSize->setCurrentText(node2.toElement().attribute("product_size"));
						//ui.cbx_recipeLayout->setCurrentText(node2.toElement().attribute("product_layout"));
						ui.cbx_recipeID->setCurrentText(node2.toElement().attribute("recipeID"));
						ui.LE_recipeUpdateTime->setText(node2.toElement().attribute("updateTime"));
						ui.LE_recipeRemark->setText(node2.toElement().attribute("remark"));

						QDomNode node3 = node2.firstChild();
						while (!node3.isNull())
						{
							if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname")=="GLassLayout")
							{
								ui.widget_recipe->clearPanelInfoList();
								//ui.cbx_recipeProductSize->setCurrentText(node2.toElement().attribute("name"));
								ui.cbx_recipeLayout->setCurrentText(node3.toElement().attribute("name"));
								LayoutPara tempLP;
								LayoutLoadByNameAndWidget(node3.toElement().attribute("name"), *ui.widget_recipe, tempLP, true);
							}
							if (node3.nodeName() == "PanelRecipe" && node3.toElement().attribute("plugname") == "OperationPlug")
							{
								QDomNode node4 = node3.firstChild();
								int j = 0;
								while (!node4.isNull())
								{
									if (node4.nodeName() == "Panel")
									{
										if (node4.nodeName() == "Panel")
										{
											QComboBox* valuebox = new QComboBox;
											ui.tableWidget_recipeAlgBind->setRowCount(j + 1);
											ui.tableWidget_recipeAlgBind->setItem(j, 0, new QTableWidgetItem(node4.toElement().attribute("panelId")));
											////ui.Table_PanelAlgBind->setItem(j, 1, new QTableWidgetItem(node4.toElement().attribute("name")));
											//QMap<QString, ALG_Parameter>::const_iterator it = algMap.constBegin();
											//while (it != algMap.constEnd())
											//{
											//	valuebox->addItem((QString)it.key());
											//	++it;
											//}
											valuebox->setCurrentText(node4.toElement().attribute("name"));
											ui.tableWidget_recipeAlgBind->setCellWidget(j, 1, valuebox);
											valuebox->setEnabled(false);
											j++;
										}
									}
									node4 = node4.nextSibling();
								}
							}
							
							node3 = node3.nextSibling();
						}

					}
					node2 = node2.nextSibling();
				}
			}
			node1 = node1.nextSibling();
		}
	}
	int rowcount2 = 0;
	int colJ = 0;
	int i = 0;
	QMap<QString, QString>::Iterator it = panelAlg.begin();
	//while (it!= panelAlg.end())
	//{
	//	if (i % 6 == 0)
	//	{
	//		++rowcount2;
	//		ui.tableWidget_CurrentRecipeAlg->setRowCount(rowcount2 + 1);
	//		colJ = 0;
	//	}
	//	else
	//	{
	//		QString sk = it.key();
	//		ui.tableWidget_CurrentRecipeAlg->setItem(rowcount2 - 1, colJ, new QTableWidgetItem(sk));
	//		++colJ;
	//		QString sv = it.value();
	//		ui.tableWidget_CurrentRecipeAlg->setItem(rowcount2 - 1, colJ, new QTableWidgetItem(sv));
	//		++colJ;
	//		it++;
	//		//ui.tableWidget_CurrentRecipeAlg->setCellWidget(rowcount2, 1, cb5);
	//	}
	//	++i;
	//}
}

void HXGD::ReadRecipeByName(QString recipeName, RecipePara& recipePara)
{
	QFile file(config_info.receipe_xml);  //关联文件名字
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
			if (node1.nodeName() == "RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				int i = 0;

				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe" && node2.toElement().attribute("name") == recipeName)
					{
						//记录当前Recipe信息
						QDomNode node3 = node2.firstChild();

						while (!node3.isNull())
						{
							if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "GLassLayout")
							{
								//GlassLayout加载
								recipePara.layoutName = node3.toElement().attribute("name");
							}
							else if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "IgnoreMask")
							{
								//IgnoreMask加载
								/*recipePara.shieldList.clear();
								QDomNode node4 = node3.firstChild();

								while (!node4.isNull())
								{
									cv::Rect shieldRect;
									if (node4.nodeName() == "Shield")
									{
										shieldRect.x = node4.toElement().attribute("x").toInt();
										shieldRect.y = node4.toElement().attribute("y").toInt();
										shieldRect.width = node4.toElement().attribute("width").toInt();
										shieldRect.height = node4.toElement().attribute("height").toInt();
										recipePara.shieldList.append(shieldRect);
									}
									node4 = node4.nextSibling();
								}*/
							}
							else if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "等级定义")
							{
								//等级定义加载

							}
							else if (node3.nodeName() == "PanelRecipe" && node3.toElement().attribute("plugname") == "OperationPlug")
							{
								//recipePara.layout.listPanel.clear();
								//OperationPlug加载
								/*QDomNode node4 = node3.firstChild();
								int j = 0;
								while (!node4.isNull())
								{
									if (node4.nodeName() == "Panel")
									{
										PanelPara pp;
										pp.panelID = node4.toElement().attribute("panelId");
										pp.AlgName = node4.toElement().attribute("name");
										rp.layout.listPanel.push_back(pp);
										j++;
									}
									node4 = node4.nextSibling();
								}*/
							}
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

}
void HXGD::LoadCurrentRecipe()
{
	//Recipe下参数读取
	QFile file(config_info.receipe_xml);  //关联文件名字
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
			if (node1.nodeName() == "RecipeIDBinding")
			{
				int j = 0;
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (g_params.systempara->CurrentRecipeID == j)
					{
						currentRecipe.recipeID = g_params.systempara->CurrentRecipeID;
						currentRecipe.recipeName = node2.toElement().attribute("bind_recipe");
						ui.LB_mainRecipeName->setText(currentRecipe.recipeName);
						ui.LB_mainRecipeID->setText(QString("000%1").arg(currentRecipe.recipeID));
						ui.LB_recipeCurrentID->setText(QString("000%1").arg(currentRecipe.recipeID));

						break;
					}
					j += 1;
					node2 = node2.nextSibling();
				}

			}
			node1 = node1.nextSibling();
		}
	}
	else {  //没有子节点
		return;
	}
	//获取根节点
	root = doc.documentElement();
	if (root.hasChildNodes()) {  //有没有子节点
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName() == "RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				int i = 0;

				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe")
					{
						ui.tableWidget_RecipeRepos->setRowCount(i + 1);
						ui.tableWidget_RecipeRepos->setItem(i, 0, new QTableWidgetItem(node2.toElement().attribute("name")));
						ui.tableWidget_RecipeRepos->setItem(i, 2, new QTableWidgetItem(node2.toElement().attribute("recipeID")));
						ui.tableWidget_RecipeRepos->setItem(i, 3, new QTableWidgetItem(node2.toElement().attribute("updateTime")));
						ui.tableWidget_RecipeRepos->setItem(i, 4, new QTableWidgetItem(node2.toElement().attribute("remark")));

						i++;
					}
					if (node2.nodeName() == "Recipe" && node2.toElement().attribute("name") == currentRecipe.recipeName)
					{
						//ui.Lab_ProRecipeInf->setText(node2.toElement().attribute("remark"));
						//ui.Lab_ProProductID->setText("Test");
						ui.LB_mainupdateTime->setText(node2.toElement().attribute("updateTime"));
						//记录当前Recipe信息
						ui.LE_recipeName->setText(node2.toElement().attribute("name"));
						ui.LE_recipeRemark->setText(node2.toElement().attribute("remark"));
						currentRecipe.ilightValue = node2.toElement().attribute("lightValue").toInt();
						currentRecipe.iMaxWarnValue = node2.toElement().attribute("warnNum").toInt();

						QDomNode node3 = node2.firstChild();

						while (!node3.isNull())
						{
							if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "GLassLayout")
							{
								ui.tableWidget_RecipeRepos->setItem(i, 1, new QTableWidgetItem(node2.toElement().attribute("name")));//获得layoutname
								ui.LB_mainRecipeLayout->setText(node3.toElement().attribute("name"));
								//GlassLayout加载
								ui.widget_layoutbar->clearPanelInfoList();
								/*ui.widget_layoutbar->clearRectBuleList();
								ui.widget_layoutbar->clearRectRedList();
								ui.widget_layoutbar->clearRectYeList();*/

								LayoutPara tempLP;
								LayoutLoadByNameAndWidget(node3.toElement().attribute("name"), *ui.widget_layoutbar, tempLP, true);
							}
							else if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "IgnoreMask")
							{
								//IgnoreMask加载

							}
							else if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "等级定义")
							{
								//等级定义加载
								//LoadDefectFilterByName(node3.toElement().attribute("name"));
							}
							else if (node3.nodeName() == "PanelRecipe" && node3.toElement().attribute("plugname") == "OperationPlug")
							{
								//OperationPlug加载
								QDomNode node4 = node3.firstChild();

								while (!node4.isNull())
								{
									if (node4.nodeName() == "Panel")
									{
										for (size_t idIndex = 0; idIndex < currentRecipe.layout.listPanel.size(); idIndex++)
										{
											QString panelIdStr = node4.toElement().attribute("panelId");
											if (currentRecipe.layout.listPanel[idIndex].panelID == node4.toElement().attribute("panelId"))
											{
												currentRecipe.layout.listPanel[idIndex].AlgName = node4.toElement().attribute("name");
											}
											else {
												currentRecipe.layout.listPanel[idIndex].AlgName = "test01";
											}
										}
									}
									node4 = node4.nextSibling();
								}
							}

							node3 = node3.nextSibling();
						}
					}

					node2 = node2.nextSibling();
				}
			}
			else if (node1.nodeName() == "RecipeIdBinding")
			{
				int j = 0;
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					ui.tableWidget_RecipeIDInfo->setRowCount(j + 1);
					ui.tableWidget_RecipeIDInfo->setItem(j, 0, new QTableWidgetItem(QString("000%1").arg(j)));
					ui.tableWidget_RecipeIDInfo->setItem(j, 1, new QTableWidgetItem(node2.toElement().attribute("bind_recipe")));

					j += 1;
					node2 = node2.nextSibling();
				}

			}
			node1 = node1.nextSibling();
		}
	}
	else {  //没有子节点
		return;
	}

}

void HXGD::on_pb_addRecipe_clicked()
{
	if (mCurrentRecipeRow == -1) return;
	QString ParaName1 = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow, 0)->text();
	int index = 1;
	QString ParaName = QString("新建配方%1").arg(index);
	while (true)
	{
		bool bSameName = false;
		for (size_t i = 0; i < ui.tableWidget_RecipeRepos->rowCount(); i++)
		{
			if (ParaName == ui.tableWidget_RecipeRepos->item(i, 0)->text())
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
			ParaName = QString("新建配方%1").arg(index);
			
		}
	};
	QString ParaTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	//QString ParamSize = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow,1)->text();
	QString ParamLayout = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow,1)->text();
	QString ParamRemarks = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow,4)->text();
	QString ParamRecipeID = "";

	ui.tableWidget_RecipeRepos->setRowCount(ui.tableWidget_RecipeRepos->rowCount() + 1);
	ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 0, new QTableWidgetItem(ParaName));
	//ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 1, new QTableWidgetItem(ParamSize));
	ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 1, new QTableWidgetItem(ParamLayout));
	ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 2, new QTableWidgetItem(ParamRecipeID));
	ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 3, new QTableWidgetItem(ParaTime));
	ui.tableWidget_RecipeRepos->setItem(ui.tableWidget_RecipeRepos->rowCount() - 1, 4, new QTableWidgetItem(ParamRemarks));
	//将数据存储到xml中
	//QString tempstr = QString("%1/data/Recipe.xml").arg(QApplication::applicationDirPath());
	QFile file(config_info.receipe_xml);  //关联文件名字
   //只读方式打开文件
	bool isOK = file.open(QIODevice::ReadOnly | QFile::Text);
	//文件file和xml关联
	QDomDocument doc;
	isOK = doc.setContent(&file);
	if (false == isOK) {

	}
	//关联成功后，可以关闭文件
	file.close();
	//获取根节点
	QDomElement root = doc.documentElement();
	QString test1 = root.nodeName();
	if (root.hasChildNodes()) {  //有没有子节点
		QDomNode node1 = root.firstChild();
		QDomNode copyNode;
		while (!node1.isNull())
		{
			if (node1.nodeName() == "RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe" && node2.toElement().attribute("name") == ParaName1)
					{
						copyNode = node2.cloneNode();
						copyNode.toElement().setAttribute("name", ParaName);
						copyNode.toElement().setAttribute("updateTime", ParaTime);
						copyNode.toElement().setAttribute("remark", "");
						//copyNode.toElement().setAttribute("product_size", ParamSize);
						copyNode.toElement().setAttribute("recipeID", "");
						//copyNode.toElement().setAttribute("product_layout", ParamLayout);
					}

					node2 = node2.nextSibling();
				}
				node1.appendChild(copyNode);
			}
			node1 = node1.nextSibling();
		}
	}

	isOK = file.open(QIODevice::WriteOnly | QFile::Truncate);

	QTextStream ts(&file);
	ts.reset();
	ts.setCodec("utf-8");
	doc.save(ts, 4, QDomNode::EncodingFromTextStream);
	//emit sigOPLogAdd(QString("新增配方 %1 成功！").arg(ParaName));
	//关联成功后，可以关闭文件
	file.close();
	ui.tableWidget_RecipeRepos->selectRow(mCurrentRecipeRow + 1);
	ui.tableWidget_RecipeRepos->setFocus();
	ui.LE_recipeName->setText(ParaName);
	//ui.cbx_recipeProductSize->setCurrentText(ParamSize);
	ui.cbx_recipeLayout->setCurrentText(ParamLayout);
	ui.cbx_recipeID->setCurrentText(ParamRecipeID);
	ui.LE_recipeRemark->setText(ParamRemarks);

}

void HXGD::on_pb_delRecipe_clicked()
{
	if (mCurrentRecipeRow >= ui.tableWidget_RecipeRepos->rowCount() || mCurrentRecipeRow < 0)
	{
		return;
	}
	QString ParaName = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow, 0)->text();
	switch (QMessageBox::information(this, tr("Delete"),
		tr(QString("确定删除配方:%1吗？").arg(ParaName).toUtf8().constData()), tr("是"), tr("否"), 0, 1))
	{
	case 0:
		break;
	case 1:
	default:
		return;
	}


	//删除参数组
	QFile file(config_info.receipe_xml);  //关联文件名字
   //只读方式打开文件
	bool isOK = file.open(QIODevice::ReadOnly | QFile::Text);

	//文件file和xml关联
	QDomDocument doc;
	isOK = doc.setContent(&file);
	if (false == isOK) {

	}
	//关联成功后，可以关闭文件
	file.close();
	//获取根节点
	QDomElement root = doc.documentElement();
	QString test1 = root.nodeName();
	if (root.hasChildNodes()) {  //有没有子节点
		QDomNode node1 = root.firstChild();
		QDomNode copyNode;
		while (!node1.isNull())
		{
			if (node1.nodeName() == "RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "Recipe" && node2.toElement().attribute("name") == ParaName)
					{
						//删除当前参数组
						copyNode = node2;
					}

					node2 = node2.nextSibling();
				}
				node1.removeChild(copyNode);
			}
			node1 = node1.nextSibling();
		}
	}
	isOK = file.open(QIODevice::WriteOnly | QFile::Truncate);

	QTextStream ts(&file);
	ts.reset();
	ts.setCodec("utf-8");
	doc.save(ts, 4, QDomNode::EncodingFromTextStream);
	//emit sigOPLogAdd(QString("删除配方 %1 成功！").arg(ParaName));
	//关联成功后，可以关闭文件
	file.close();
	ui.tableWidget_RecipeRepos->removeRow(mCurrentRecipeRow);
	mCurrentRecipeRow = -1;
}
void HXGD::on_pb_recipeEdit_clicked()
{
}
void HXGD::on_pb_recipeSave_clicked()
{
	QString ParaName = ui.tableWidget_RecipeRepos->item(mCurrentRecipeRow, 0)->text();
	RecipePara tempPP;
	tempPP.recipeName = ParaName;
	switch (QMessageBox::information(this, tr("修改"),
		tr(QString("确定修改配方吗:%1吗？").arg(ParaName).toUtf8().constData()), tr("是"), tr("否"), 0, 1))
	{
	case 0:
		break;
	case 1:
	default:
		return;
	}

	QString recipe_updateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString recipeName = ui.LE_recipeName->text();
	//QString recipeLayoutSize = ui.cbx_recipeLayout->currentText();
	QString recipeLayout = ui.cbx_recipeLayout->currentText();
	QString recipeID = ui.cbx_recipeID->currentText();
	QString recipeRemark = ui.LE_recipeRemark->text();

	ReadRecipeByName(tempPP.recipeName, tempPP);
	QFile file(config_info.receipe_xml);  //关联文件名字
   //只读方式打开文件
	bool isOK = file.open(QIODevice::ReadOnly);

	//文件file和xml关联
	QDomDocument doc;
	isOK = doc.setContent(&file);
	if (false == isOK) {

	}
	//关联成功后，可以关闭文件
	file.close();
	QStringList tempdata;
	//获取根节点
	QDomElement root = doc.documentElement();
	QString test1 = root.nodeName();
	if (root.hasChildNodes())
	{
		QDomNode node1 = root.firstChild();
		while (!node1.isNull())
		{
			if (node1.nodeName()=="RecipeList")
			{
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName()=="Recipe"&&node2.toElement().attribute("name")==ParaName)
					{
						if (tempPP.recipeName!=recipeName)
						{
							node2.toElement().setAttribute("name", recipeName);
						}
						node2.toElement().setAttribute("updateTime",recipe_updateTime);
						node2.toElement().setAttribute("remark", recipeRemark);
						node2.toElement().setAttribute("recipeID",recipeID);

						QDomNode node3 = node2.firstChild();

						while (!node3.isNull())
						{
							if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "GLassLayout")
							{
								if (tempPP.layoutName != recipeLayout)
								{
									node3.toElement().setAttribute("name", recipeLayout);
									node3.toElement().setAttribute("updateTime", recipe_updateTime);
								}
							}
							if (node3.nodeName() == "BaseRecipe" && node3.toElement().attribute("plugname") == "IgnoreMask")
							{

							}

							node3 = node3.nextSibling();
						}
					}
					node2 = node2.nextSibling();
				}
			}
			if (node1.nodeName() == "RecipeIDBinding"  )
			{
				QDomNode node2 = node1.firstChild();
				while (!node2.isNull())
				{
					if (node2.nodeName() == "RecipeID" && node2.toElement().attribute("recipe_id") == ui.cbx_recipeID->currentText())
					{
						QString s = ui.LE_recipeName->text();
						node2.toElement().setAttribute("bind_recipe", ui.LE_recipeName->text());
					}
					node2 = node2.nextSibling();
				}
			}
			node1 = node1.nextSibling();
		}

	}
	isOK = file.open(QIODevice::WriteOnly | QFile::Truncate);

	QTextStream ts(&file);
	ts.reset();
	ts.setCodec("utf-8");
	doc.save(ts, 4, QDomNode::EncodingFromTextStream);
	//关联成功后，可以关闭文件
	file.close();

	LoadAllRecipes();
}
void HXGD::on_pb_recipeCancel_clicked()
{

}
void HXGD::slotRecipeIDCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
}

void HXGD::slotCurrentRecipeInfoCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
	
}

void HXGD::slotCurrentRecipeReposCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
	mCurrentRecipeRow = currentRow;
	currentRecipeName = ui.tableWidget_RecipeRepos->item(currentRow,0)->text();
	currentSize = ui.tableWidget_RecipeRepos->item(currentRow, 1)->text();
	//currentLayout = ui.tableWidget_RecipeRepos->item(currentRow, 2)->text();
	ShowCurrentRecipeInfo(currentRecipeName,currentSize, currentLayout);
}



