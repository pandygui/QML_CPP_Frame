#include "xyqmlsinglerealobject.h"
#include "xystudentinfo.h"
#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QFileDialog>
#include <QImage>

XYQmlSingleRealObject *XYQmlSingleRealObject::mopInstance = 0;

XYQmlSingleRealObject::XYQmlSingleRealObject(QObject *parent) : QObject(parent)
{
    moduleList.append(QStringLiteral("��Ա��Ϣ"));
    moduleList.append(QStringLiteral("��ѵ��Ϣ"));
    moduleList.append(QStringLiteral("������Ϣ"));
    moduleList.append(QStringLiteral("��ѯ��Ϣ"));
    moduleList.append(QStringLiteral("��Ϣͳ��"));
    moduleList.append(QStringLiteral("�����"));
    moduleList.append(QStringLiteral("�ǩ��"));
    moduleList.append(QStringLiteral("��ӡ֤��"));

    for (int i = 0; i < moduleList.size(); ++i)
    {
        switch (i) {
        case 0:
            modeleInfo.insert(moduleList.at(i), "StudentInfo.qml");
            break;
        case 1:
            modeleInfo.insert(moduleList.at(i), "CourseInfo.qml");
            break;
        case 2:
            modeleInfo.insert(moduleList.at(i), "ServeInfo.qml");
            break;
        case 7:
            modeleInfo.insert(moduleList.at(i), "PrintPage.qml");
            break;
        default:
            modeleInfo.insert(moduleList.at(i), "DefaultInfo.qml");
            break;
        }
    }
    // ����������ݿ��ȡ��Ϣ��ʼ��model����
    for (int i = 0; i < 13; ++i )
    {
        QString s_image = QString("tem/%1.png").arg(i + 1);
        certificateTemplateModel.append(s_image);
        mlstImages.append(new QImage(":/" + s_image));
    }
}

XYQmlSingleRealObject *XYQmlSingleRealObject::getInstance()
{
    if (mopInstance == 0)
    {
        mopInstance = new XYQmlSingleRealObject;
    }
    return mopInstance;
}

QString XYQmlSingleRealObject::getString(int index)
{
    QString fileName = "123";
    if (fileName.isEmpty())
        return "";
    if (!fileName.endsWith(QLatin1String(".rtf")))
        fileName += QLatin1String(".rtf");

    // �����ǿ���룬��Ҫ�����û��ֶ�ѡ�񱣴�·���� �Լ������ļ����� ����û�û�м��Ϻ�׺���Զ������ļ���׺��
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        return "";
    }
    // ������һС�Σ����ı��ķ�ʽ���򿪲���д�ļ���

    QDomDocument doc;  //xml��rtf �ȶ����ı���ʽ��������Ҫ���ࡣ
    doc.setContent(&file);
    QDomElement root = doc.documentElement();
    QDomElement body;
    if (!root.isNull()) {
        body = root.firstChildElement(QLatin1String("body"));  //����û�������ļ����������ݣ� �����Ƿ���rtf��ʽ�����Ԫ�ء�
    }
    if (body.isNull()) {
        root = doc.createElement(QLatin1String("html"));
        QDomElement head = doc.createElement(QLatin1String("head"));
        QDomElement meta = doc.createElement(QLatin1String("meta"));
        meta.setAttribute(QLatin1String("http-equiv"), QLatin1String("Content-Type"));
        meta.setAttribute(QLatin1String("content"), QLatin1String("text/html; charset=utf-8"));
        //rtf��ʽ����һ��html��ʽ�ļ��� һ���Ľṹ�� ��Ҫ�ļ�ͷ�� body���ݡ�
        head.appendChild(meta);
        root.appendChild(head);
        body = doc.createElement(QLatin1String("body"));
        root.appendChild(body);
        doc.appendChild(root);
    }
    // ������һС����Ҫд�ļ����ļ�ͷ�����ݡ�
    if (body.isNull())
        return "";
    QDomElement subtitleParagraph = doc.createElement(QLatin1String("p"));
    QDomText subtitleText = doc.createTextNode(tr("Subtitle:")+QLatin1String("this is a test"));
    // ������һС�飬 ͨ��һ����ͨ��p��ǩ����ʵ��body ��һ���ݡ�
    subtitleParagraph.appendChild(subtitleText);
    QDomElement tempLabel = doc.createElement(QLatin1String("span"));
    tempLabel.setAttribute(QLatin1String("style"), QLatin1String("color:red"));
    QDomText tempNode = doc.createTextNode(QLatin1String("  this is red"));
    tempLabel.appendChild(tempNode);
    subtitleParagraph.appendChild(tempLabel);
    // ������һ�����ݣ�������p��ǩ�� ����һ�����ı���ʽ����<span>��ǩ �� ��������ַ������ݱ�Ϊ��ɫ��
    body.appendChild(subtitleParagraph);
    QTextStream out(&file);
    doc.save(out, 4);

    file.close();
    return doc.toString();
}
