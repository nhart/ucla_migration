<xsl:stylesheet xmlns:mods="http://www.loc.gov/mods/v3"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema"
                exclude-result-prefixes="xs" version="1.0">

    <xsl:output omit-xml-declaration="yes" indent="yes"/>
    <xsl:strip-space elements="*"/>
    <xsl:param name="old_value"/>
    <xsl:param name="new_value"/>
    <xsl:param name="update_element"/>

    <xsl:template match="node()|@*">
        <xsl:choose>
            <xsl:when test="self::* and local-name()=$update_element">
                <xsl:copy>
                    <xsl:apply-templates select="@*|node()"/>
                </xsl:copy>
            </xsl:when>
            <xsl:when test="self::text() and self::text()=$old_value">
                <xsl:value-of select="$new_value"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:copy>
                    <xsl:apply-templates select="@*|node()"/>
                </xsl:copy>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>




</xsl:stylesheet>





