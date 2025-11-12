#include <stdio.h>
int main(){
    FILE*f=fopen("out.txt","wb");
    if(!f) return 1;
    fputs(R"(class FlowItem : public QGraphicsPathItem {
	public:
	FlowItem(FlowItemType _type);
	virtual ~FlowItem() override;
	public:
	virtual bool handleDropEvent() = 0;
	virtual bool handleExecuteEvent() = 0;
	virtual bool handleOpenResultsEvent() = 0;
	virtual bool handleResetEvent() = 0;
	virtual bool handleShowPropertiesEvent() = 0;
	virtual QString getInfoString() = 0;
	virtual QString getItemTypeAsString() = 0;
};

class FlowItem_Import : public FlowItem {
	public:
	FlowItem_Import();
	~FlowItem_Import() override final;
	public:
	bool handleDropEvent() override final;
	bool handleExecuteEvent() override final;
	bool handleOpenResultsEvent() override final;
	bool handleResetEvent() override final;
	bool handleShowPropertiesEvent() override final;
	QString getInfoString() override final;
	QString getItemTypeAsString() override final;
};

class FlowItem_Export : public FlowItem {
	public:
	FlowItem_Export();
	~FlowItem_Export() override final;
	public:
	bool handleDropEvent() override final;
	bool handleExecuteEvent() override final;
	bool handleOpenResultsEvent() override final;
	bool handleResetEvent() override final;
	bool handleShowPropertiesEvent() override final;
	QString getInfoString() override final;
	QString getItemTypeAsString() override final;
};

class FlowItem_Edit : public FlowItem {
	public:
	FlowItem_Edit();
	~FlowItem_Edit() override final;
	public:
	bool handleDropEvent() override final;
	bool handleExecuteEvent() override final;
	bool handleOpenResultsEvent() override final;
	bool handleResetEvent() override final;
	bool handleShowPropertiesEvent() override final;
	QString getInfoString() override final;
	QString getItemTypeAsString() override final;
};
)",f);
    fclose(f);
    return 0;
}