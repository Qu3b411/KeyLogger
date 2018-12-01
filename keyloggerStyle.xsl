<?xml version="1.0" encoding="UTF-8"?>
<html xsl:version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<body style="font-family:Arial;font-size:12pt;background-color:#EEEEEE">
<xsl:for-each select="KeyLoggerMetaData/Process">
  <div style="background-color:teal;color:white;padding:4px">
  Process Handle:
    <xsl:value-of select="ProcessHandle"/>
<div></div>
Process ID:
    <xsl:value-of select="ProcessID"/>

    </div>

<xsl:for-each select="current()/title">
  <div style="background-color:red;color:white;padding:4px">
Process Title: 
    <xsl:value-of select="ProcessTitle"/>
    <div></div>
Time Stamp:
    <xsl:value-of select="TimeStamp"/>
    <div></div>
 </div>
  <div style="margin-left:20px;margin-bottom:1em;font-size:10pt">
    <p>
    <xsl:value-of select="keystrokes"/>
    </p>
  </div>
</xsl:for-each>
</xsl:for-each>
</body>
</html>
