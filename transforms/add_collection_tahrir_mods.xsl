<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:mods="http://www.loc.gov/mods/v3"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema"
    exclude-result-prefixes="xs" version="1.0">
    <xsl:output omit-xml-declaration="yes" indent="yes"/>
    <xsl:strip-space elements="*"/>

    <xsl:template match="node() | @*">
        <xsl:copy>
            <xsl:apply-templates select="node() | @*"/>
        </xsl:copy>

    </xsl:template>
    <xsl:template match="mods:mods">
        <xsl:copy>
            <xsl:apply-templates select="node() | @*"/>
            <mods:relatedItem type="host">
                <mods:titleInfo>
                    <mods:title>Tahrir Square</mods:title>
                </mods:titleInfo>
            </mods:relatedItem>
         </xsl:copy>

    </xsl:template>



</xsl:stylesheet>

