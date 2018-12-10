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
   <xsl:for-each select="current()/logged">
<p>
<xsl:choose>
<xsl:when test="CaptureType='Keylogger'">
<div style="background-color:gold;color:black;padding:4px">
    <xsl:value-of select="CaptureType"/> </div>
<div style="background-color:yellow;color:black;padding:4px">
    <xsl:value-of select="Capture"/> </div>
</xsl:when>
<xsl:otherwise>
<div style="background-color:blue;color:black;padding:4px">
    <xsl:value-of select="CaptureType"/> </div>

<div style="background-color:aqua;color:black;padding:4px">
    <xsl:value-of select="Capture"/> </div>
</xsl:otherwise>
</xsl:choose>
    </p>
</xsl:for-each>
    </p>
  </div>
</xsl:for-each>
</xsl:for-each>
</body>
</html>
