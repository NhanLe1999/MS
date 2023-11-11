package org.cocos2dx.cpp;

public class InfoConnection {
    String _url;
    String _pathFile;

    InfoConnection(String url, String pathFile)
    {
        this._url = url;
        this._pathFile = pathFile;
    }

    public String getUrl()
    {
        return this._url;
    }

    public String getPathFile()
    {
        return this._pathFile;
    }
}
