// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QXML_H
#define QXML_H

#include <QtCore5Compat/qcore5global.h>

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qlist.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QXmlNamespaceSupport;
class QXmlAttributes;
class QXmlContentHandler;
class QXmlDefaultHandler;
class QXmlDTDHandler;
class QXmlEntityResolver;
class QXmlErrorHandler;
class QXmlLexicalHandler;
class QXmlDeclHandler;
class QXmlInputSource;
class QXmlLocator;
class QXmlNamespaceSupport;
class QXmlParseException;

class QXmlReader;
class QXmlSimpleReader;

class QXmlSimpleReaderPrivate;
class QXmlNamespaceSupportPrivate;
class QXmlAttributesPrivate;
class QXmlInputSourcePrivate;
class QXmlParseExceptionPrivate;
class QXmlLocatorPrivate;
class QXmlDefaultHandlerPrivate;

//
// SAX Namespace Support
//

class Q_CORE5COMPAT_EXPORT QXmlNamespaceSupport
{
public:
    QXmlNamespaceSupport();
    ~QXmlNamespaceSupport();

    void setPrefix(const QString&, const QString&);

    QString prefix(const QString&) const;
    QString uri(const QString&) const;
    void splitName(const QString&, QString&, QString&) const;
    void processName(const QString&, bool, QString&, QString&) const;
    QStringList prefixes() const;
    QStringList prefixes(const QString&) const;

    void pushContext();
    void popContext();
    void reset();

private:
    QXmlNamespaceSupportPrivate *d;

    friend class QXmlSimpleReaderPrivate;
    Q_DISABLE_COPY(QXmlNamespaceSupport)
};

//
// SAX Attributes
//

class Q_CORE5COMPAT_EXPORT QXmlAttributes
{
public:
    QXmlAttributes();
    QXmlAttributes(const QXmlAttributes &) = default;
    QXmlAttributes(QXmlAttributes &&) noexcept = default;
    QXmlAttributes &operator=(const QXmlAttributes &) = default;
    QXmlAttributes &operator=(QXmlAttributes &&) noexcept = default;

    virtual ~QXmlAttributes();

    void swap(QXmlAttributes &other) noexcept
    {
        attList.swap(other.attList);
        qt_ptr_swap(d, other.d);
    }

    int index(const QString& qName) const;
    int index(QLatin1String qName) const;
    int index(const QString& uri, const QString& localPart) const;
    int length() const;
    int count() const;
    QString localName(int index) const;
    QString qName(int index) const;
    QString uri(int index) const;
    QString type(int index) const;
    QString type(const QString& qName) const;
    QString type(const QString& uri, const QString& localName) const;
    QString value(int index) const;
    QString value(const QString& qName) const;
    QString value(QLatin1String qName) const;
    QString value(const QString& uri, const QString& localName) const;

    void clear();
    void append(const QString &qName, const QString &uri, const QString &localPart, const QString &value);

private:
    struct Attribute {
        QString qname, uri, localname, value;
    };
    friend class QTypeInfo<Attribute>;
    typedef QList<Attribute> AttributeList;
    AttributeList attList;

    QXmlAttributesPrivate *d;
};

Q_DECLARE_TYPEINFO(QXmlAttributes::Attribute, Q_RELOCATABLE_TYPE);
Q_DECLARE_SHARED(QXmlAttributes)

//
// SAX Input Source
//

class Q_CORE5COMPAT_EXPORT QXmlInputSource
{
public:
    QXmlInputSource();
    explicit QXmlInputSource(QIODevice *dev);
    virtual ~QXmlInputSource();

    virtual void setData(const QString& dat);
    virtual void setData(const QByteArray& dat);
    virtual void fetchData();
    virtual QString data() const;
    virtual QChar next();
    virtual void reset();

    static const char16_t EndOfData;
    static const char16_t EndOfDocument;

protected:
    virtual QString fromRawData(const QByteArray &data, bool beginning = false);

private:
    void init();
    QXmlInputSourcePrivate *d;
};

//
// SAX Exception Classes
//

class Q_CORE5COMPAT_EXPORT QXmlParseException
{
public:
    explicit QXmlParseException(const QString &name = QString(), int c = -1, int l = -1,
                                const QString &p = QString(), const QString &s = QString());
    QXmlParseException(const QXmlParseException &other);
    ~QXmlParseException();

    int columnNumber() const;
    int lineNumber() const;
    QString publicId() const;
    QString systemId() const;
    QString message() const;

private:
    std::unique_ptr<QXmlParseExceptionPrivate> d;
};

//
// XML Reader
//

class Q_CORE5COMPAT_EXPORT QXmlReader
{
public:
    virtual ~QXmlReader();
    virtual bool feature(const QString& name, bool *ok = nullptr) const = 0;
    virtual void setFeature(const QString& name, bool value) = 0;
    virtual bool hasFeature(const QString& name) const = 0;
    virtual void* property(const QString& name, bool *ok = nullptr) const = 0;
    virtual void setProperty(const QString& name, void* value) = 0;
    virtual bool hasProperty(const QString& name) const = 0;
    virtual void setEntityResolver(QXmlEntityResolver* handler) = 0;
    virtual QXmlEntityResolver* entityResolver() const = 0;
    virtual void setDTDHandler(QXmlDTDHandler* handler) = 0;
    virtual QXmlDTDHandler* DTDHandler() const = 0;
    virtual void setContentHandler(QXmlContentHandler* handler) = 0;
    virtual QXmlContentHandler* contentHandler() const = 0;
    virtual void setErrorHandler(QXmlErrorHandler* handler) = 0;
    virtual QXmlErrorHandler* errorHandler() const = 0;
    virtual void setLexicalHandler(QXmlLexicalHandler* handler) = 0;
    virtual QXmlLexicalHandler* lexicalHandler() const = 0;
    virtual void setDeclHandler(QXmlDeclHandler* handler) = 0;
    virtual QXmlDeclHandler* declHandler() const = 0;
    virtual bool parse(const QXmlInputSource& input) = 0;
    virtual bool parse(const QXmlInputSource* input) = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlSimpleReader
    : public QXmlReader
{
public:
    QXmlSimpleReader();
    virtual ~QXmlSimpleReader();

    bool feature(const QString& name, bool *ok = nullptr) const override;
    void setFeature(const QString& name, bool value) override;
    bool hasFeature(const QString& name) const override;

    void* property(const QString& name, bool *ok = nullptr) const override;
    void setProperty(const QString& name, void* value) override;
    bool hasProperty(const QString& name) const override;

    void setEntityResolver(QXmlEntityResolver* handler) override;
    QXmlEntityResolver* entityResolver() const override;
    void setDTDHandler(QXmlDTDHandler* handler) override;
    QXmlDTDHandler* DTDHandler() const override;
    void setContentHandler(QXmlContentHandler* handler) override;
    QXmlContentHandler* contentHandler() const override;
    void setErrorHandler(QXmlErrorHandler* handler) override;
    QXmlErrorHandler* errorHandler() const override;
    void setLexicalHandler(QXmlLexicalHandler* handler) override;
    QXmlLexicalHandler* lexicalHandler() const override;
    void setDeclHandler(QXmlDeclHandler* handler) override;
    QXmlDeclHandler* declHandler() const override;

    bool parse(const QXmlInputSource& input) override;
    bool parse(const QXmlInputSource* input) override;
    virtual bool parse(const QXmlInputSource* input, bool incremental);
    virtual bool parseContinue();

private:
    Q_DISABLE_COPY(QXmlSimpleReader)
    Q_DECLARE_PRIVATE(QXmlSimpleReader)
    std::unique_ptr<QXmlSimpleReaderPrivate> d_ptr;

    friend class QXmlSimpleReaderLocator;
};

//
// SAX Locator
//

class Q_CORE5COMPAT_EXPORT QXmlLocator
{
public:
    QXmlLocator();
    virtual ~QXmlLocator();

    virtual int columnNumber() const = 0;
    virtual int lineNumber() const = 0;
};

//
// SAX handler classes
//

class Q_CORE5COMPAT_EXPORT QXmlContentHandler
{
public:
    virtual ~QXmlContentHandler();
    virtual void setDocumentLocator(QXmlLocator* locator) = 0;
    virtual bool startDocument() = 0;
    virtual bool endDocument() = 0;
    virtual bool startPrefixMapping(const QString& prefix, const QString& uri) = 0;
    virtual bool endPrefixMapping(const QString& prefix) = 0;
    virtual bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) = 0;
    virtual bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName) = 0;
    virtual bool characters(const QString& ch) = 0;
    virtual bool ignorableWhitespace(const QString& ch) = 0;
    virtual bool processingInstruction(const QString& target, const QString& data) = 0;
    virtual bool skippedEntity(const QString& name) = 0;
    virtual QString errorString() const = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlErrorHandler
{
public:
    virtual ~QXmlErrorHandler();
    virtual bool warning(const QXmlParseException& exception) = 0;
    virtual bool error(const QXmlParseException& exception) = 0;
    virtual bool fatalError(const QXmlParseException& exception) = 0;
    virtual QString errorString() const = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlDTDHandler
{
public:
    virtual ~QXmlDTDHandler();
    virtual bool notationDecl(const QString& name, const QString& publicId, const QString& systemId) = 0;
    virtual bool unparsedEntityDecl(const QString& name, const QString& publicId, const QString& systemId, const QString& notationName) = 0;
    virtual QString errorString() const = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlEntityResolver
{
public:
    virtual ~QXmlEntityResolver();
    virtual bool resolveEntity(const QString& publicId, const QString& systemId, QXmlInputSource*& ret) = 0;
    virtual QString errorString() const = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlLexicalHandler
{
public:
    virtual ~QXmlLexicalHandler();
    virtual bool startDTD(const QString& name, const QString& publicId, const QString& systemId) = 0;
    virtual bool endDTD() = 0;
    virtual bool startEntity(const QString& name) = 0;
    virtual bool endEntity(const QString& name) = 0;
    virtual bool startCDATA() = 0;
    virtual bool endCDATA() = 0;
    virtual bool comment(const QString& ch) = 0;
    virtual QString errorString() const = 0;
};

class Q_CORE5COMPAT_EXPORT QXmlDeclHandler
{
public:
    virtual ~QXmlDeclHandler();
    virtual bool attributeDecl(const QString& eName, const QString& aName, const QString& type, const QString& valueDefault, const QString& value) = 0;
    virtual bool internalEntityDecl(const QString& name, const QString& value) = 0;
    virtual bool externalEntityDecl(const QString& name, const QString& publicId, const QString& systemId) = 0;
    virtual QString errorString() const = 0;
    // ### Conform to SAX by adding elementDecl
};

class Q_CORE5COMPAT_EXPORT QXmlDefaultHandler : public QXmlContentHandler,
                                                public QXmlErrorHandler,
                                                public QXmlDTDHandler,
                                                public QXmlEntityResolver,
                                                public QXmlLexicalHandler,
                                                public QXmlDeclHandler
{
public:
    QXmlDefaultHandler();
    virtual ~QXmlDefaultHandler();

    void setDocumentLocator(QXmlLocator* locator) override;
    bool startDocument() override;
    bool endDocument() override;
    bool startPrefixMapping(const QString& prefix, const QString& uri) override;
    bool endPrefixMapping(const QString& prefix) override;
    bool startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& atts) override;
    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName) override;
    bool characters(const QString& ch) override;
    bool ignorableWhitespace(const QString& ch) override;
    bool processingInstruction(const QString& target, const QString& data) override;
    bool skippedEntity(const QString& name) override;

    bool warning(const QXmlParseException& exception) override;
    bool error(const QXmlParseException& exception) override;
    bool fatalError(const QXmlParseException& exception) override;

    bool notationDecl(const QString& name, const QString& publicId, const QString& systemId) override;
    bool unparsedEntityDecl(const QString& name, const QString& publicId, const QString& systemId, const QString& notationName) override;

    bool resolveEntity(const QString& publicId, const QString& systemId, QXmlInputSource*& ret) override;

    bool startDTD(const QString& name, const QString& publicId, const QString& systemId) override;
    bool endDTD() override;
    bool startEntity(const QString& name) override;
    bool endEntity(const QString& name) override;
    bool startCDATA() override;
    bool endCDATA() override;
    bool comment(const QString& ch) override;

    bool attributeDecl(const QString& eName, const QString& aName, const QString& type, const QString& valueDefault, const QString& value) override;
    bool internalEntityDecl(const QString& name, const QString& value) override;
    bool externalEntityDecl(const QString& name, const QString& publicId, const QString& systemId) override;

    QString errorString() const override;

private:
    QXmlDefaultHandlerPrivate *d;
    Q_DISABLE_COPY(QXmlDefaultHandler)
};

// inlines

inline int QXmlAttributes::count() const
{ return length(); }

QT_END_NAMESPACE

#endif // QXML_H
