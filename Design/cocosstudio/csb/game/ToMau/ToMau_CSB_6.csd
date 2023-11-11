<GameFile>
  <PropertyGroup Name="ToMau_CSB" Type="Layer" ID="7640398a-1932-4560-a52a-30664f3fef61" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ToMau" Tag="79" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="-795817293" Tag="81" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-336.6688" RightMargin="-339.3312" TopMargin="4.3057" BottomMargin="-4.3057" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="513.3312" Y="379.6943" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5013" Y="0.4944" />
            <PreSize X="1.6602" Y="1.0000" />
            <FileData Type="Normal" Path="games/ToMau/BG.png" Plist="" />
            <BlendFunc Src="1" Dst="771" />
          </AbstractNodeData>
          <AbstractNodeData Name="root_layout" ActionTag="-356526763" Tag="80" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="speaker" ActionTag="-912679806" CallBackType="Click" CallBackName="onClickSpeaker" Tag="142" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="48.0392" RightMargin="880.9608" TopMargin="31.2968" BottomMargin="641.7032" TouchEnable="True" FontSize="14" ButtonText="Button" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="65" Scale9Height="73" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="95.0000" Y="95.0000" />
                <Children>
                  <AbstractNodeData Name="loa" ActionTag="584706909" CallBackType="Click" Tag="143" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="14.0000" RightMargin="14.0000" TopMargin="15.5000" BottomMargin="15.5000" LeftEage="22" RightEage="22" TopEage="21" BottomEage="21" Scale9OriginX="22" Scale9OriginY="21" Scale9Width="23" Scale9Height="22" ctype="ImageViewObjectData">
                    <Size X="67.0000" Y="64.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="47.5000" Y="47.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7053" Y="0.6737" />
                    <FileData Type="Normal" Path="games/ToMau/loa.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="95.5392" Y="689.2032" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0933" Y="0.8974" />
                <PreSize X="0.0928" Y="0.1237" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                <NormalFileData Type="Normal" Path="games/ToMau/icon.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="question_box" ActionTag="552559205" Tag="137" IconVisible="False" LeftMargin="164.6963" RightMargin="59.3037" TopMargin="16.4746" BottomMargin="631.5254" LeftEage="297" RightEage="297" TopEage="51" BottomEage="51" Scale9OriginX="297" Scale9OriginY="51" Scale9Width="308" Scale9Height="53" ctype="ImageViewObjectData">
                <Size X="800.0000" Y="120.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1" ActionTag="-352895011" Tag="90" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="327.5000" RightMargin="327.5000" TopMargin="39.0000" BottomMargin="39.0000" FontSize="36" LabelText="Question" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="145.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="400.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="26" G="26" B="26" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.1813" Y="0.3500" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="564.6963" Y="691.5254" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5515" Y="0.9004" />
                <PreSize X="0.7813" Y="0.1563" />
                <FileData Type="Normal" Path="games/ToMau/question.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="board" ActionTag="-2038146564" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="190.5000" RightMargin="190.5000" TopMargin="128.0000" BottomMargin="128.0000" LeftEage="274" RightEage="274" TopEage="135" BottomEage="135" Scale9OriginX="274" Scale9OriginY="135" Scale9Width="95" Scale9Height="242" ctype="ImageViewObjectData">
                <Size X="643.0000" Y="512.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6279" Y="0.6667" />
                <FileData Type="Normal" Path="games/ToMauImg/THVCT_19.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="right_panel" ActionTag="-1193336645" Tag="139" IconVisible="False" LeftMargin="58.4911" RightMargin="165.5089" TopMargin="677.4063" BottomMargin="-89.4063" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="800.0000" Y="180.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="458.4911" Y="0.5937" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4477" Y="0.0008" />
                <PreSize X="0.7813" Y="0.2344" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_2" ActionTag="1678840198" CallBackName="onTouchPanel" Tag="20" IconVisible="False" LeftMargin="303.5684" RightMargin="307.4316" TopMargin="185.0282" BottomMargin="173.9718" ClipAble="False" BackColorAlpha="162" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="413.0000" Y="409.0000" />
                <Children>
                  <AbstractNodeData Name="Image_3" ActionTag="1382762100" CallBackType="Touch" CallBackName="onTouchObject" Tag="16" IconVisible="False" LeftMargin="27.6629" RightMargin="299.3370" TopMargin="80.7060" BottomMargin="254.2940" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="56" Scale9Height="44" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="74.0000" />
                    <AnchorPoint />
                    <Position X="27.6629" Y="254.2940" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0670" Y="0.6217" />
                    <PreSize X="0.2082" Y="0.1809" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_10.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="778456474" CallBackType="Touch" CallBackName="onTouchObject" Tag="14" IconVisible="False" LeftMargin="292.9088" RightMargin="39.0912" TopMargin="79.7939" BottomMargin="259.2061" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="51" Scale9Height="40" ctype="ImageViewObjectData">
                    <Size X="81.0000" Y="70.0000" />
                    <AnchorPoint />
                    <Position X="292.9088" Y="259.2061" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7092" Y="0.6338" />
                    <PreSize X="0.1961" Y="0.1711" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_11.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" ActionTag="-1143914025" CallBackType="Touch" CallBackName="onTouchObject" Tag="17" IconVisible="False" LeftMargin="3.1078" RightMargin="346.8922" TopMargin="233.6368" BottomMargin="118.3632" TouchEnable="True" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="49" Scale9Height="41" ctype="ImageViewObjectData">
                    <Size X="63.0000" Y="57.0000" />
                    <AnchorPoint />
                    <Position X="3.1078" Y="118.3632" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0075" Y="0.2894" />
                    <PreSize X="0.1525" Y="0.1394" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_12.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0" ActionTag="1490338426" CallBackType="Touch" CallBackName="onTouchObject" Tag="80" IconVisible="False" LeftMargin="33.5588" RightMargin="325.4412" TopMargin="334.5818" BottomMargin="2.4182" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="30" Scale9Height="32" ctype="ImageViewObjectData">
                    <Size X="54.0000" Y="72.0000" />
                    <AnchorPoint />
                    <Position X="33.5588" Y="2.4182" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0813" Y="0.0059" />
                    <PreSize X="0.1308" Y="0.1760" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_13.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_0" ActionTag="-443704520" CallBackType="Touch" CallBackName="onTouchObject" Tag="92" IconVisible="False" LeftMargin="324.1431" RightMargin="33.8569" TopMargin="333.4825" BottomMargin="3.5175" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="31" Scale9Height="32" ctype="ImageViewObjectData">
                    <Size X="55.0000" Y="72.0000" />
                    <AnchorPoint />
                    <Position X="324.1431" Y="3.5175" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7849" Y="0.0086" />
                    <PreSize X="0.1332" Y="0.1760" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_14.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_1" ActionTag="209709212" CallBackType="Touch" CallBackName="onTouchObject" Tag="94" IconVisible="False" LeftMargin="346.6985" RightMargin="2.3015" TopMargin="232.8887" BottomMargin="119.1113" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="40" Scale9Height="17" ctype="ImageViewObjectData">
                    <Size X="64.0000" Y="57.0000" />
                    <AnchorPoint />
                    <Position X="346.6985" Y="119.1113" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.8395" Y="0.2912" />
                    <PreSize X="0.1550" Y="0.1394" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_15.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_3" ActionTag="1621396348" CallBackType="Touch" CallBackName="onTouchObject" Tag="199" IconVisible="False" LeftMargin="162.1165" RightMargin="161.8835" TopMargin="124.7300" BottomMargin="1.2700" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="65" Scale9Height="243" ctype="ImageViewObjectData">
                    <Size X="89.0000" Y="283.0000" />
                    <AnchorPoint />
                    <Position X="162.1165" Y="1.2700" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3925" Y="0.0031" />
                    <PreSize X="0.2155" Y="0.6919" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_18.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_7_3" ActionTag="1712470896" CallBackType="Touch" CallBackName="onTouchObject" Tag="207" IconVisible="False" LeftMargin="65.0529" RightMargin="206.9471" TopMargin="106.8390" BottomMargin="8.1610" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="117" Scale9Height="254" ctype="ImageViewObjectData">
                    <Size X="141.0000" Y="294.0000" />
                    <AnchorPoint />
                    <Position X="65.0529" Y="8.1610" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1575" Y="0.0200" />
                    <PreSize X="0.3414" Y="0.7188" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_8.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_7_2" ActionTag="-943176483" CallBackType="Touch" CallBackName="onTouchObject" Tag="206" IconVisible="False" LeftMargin="206.3089" RightMargin="65.6912" TopMargin="108.6381" BottomMargin="6.3619" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="117" Scale9Height="254" ctype="ImageViewObjectData">
                    <Size X="141.0000" Y="294.0000" />
                    <AnchorPoint />
                    <Position X="206.3089" Y="6.3619" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4995" Y="0.0156" />
                    <PreSize X="0.3414" Y="0.7188" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_9.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0" ActionTag="-549386236" CallBackType="Touch" CallBackName="onTouchObject" Tag="63" IconVisible="False" LeftMargin="105.2409" RightMargin="271.7591" TopMargin="157.9901" BottomMargin="208.0099" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="12" Scale9Height="3" ctype="ImageViewObjectData">
                    <Size X="36.0000" Y="43.0000" />
                    <AnchorPoint />
                    <Position X="105.2409" Y="208.0099" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2548" Y="0.5086" />
                    <PreSize X="0.0872" Y="0.1051" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_0.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6" ActionTag="-356835940" CallBackType="Touch" CallBackName="onTouchObject" Tag="19" IconVisible="False" LeftMargin="138.9988" RightMargin="236.0012" TopMargin="231.6607" BottomMargin="124.3393" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="14" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="38.0000" Y="53.0000" />
                    <AnchorPoint />
                    <Position X="138.9988" Y="124.3393" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3366" Y="0.3040" />
                    <PreSize X="0.0920" Y="0.1296" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_4" ActionTag="-1984556774" CallBackType="Touch" CallBackName="onTouchObject" Tag="200" IconVisible="False" LeftMargin="77.7854" RightMargin="305.2147" TopMargin="263.3183" BottomMargin="99.6817" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="6" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="46.0000" />
                    <AnchorPoint />
                    <Position X="77.7854" Y="99.6817" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1883" Y="0.2437" />
                    <PreSize X="0.0726" Y="0.1125" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_5" ActionTag="-1375092549" CallBackType="Touch" CallBackName="onTouchObject" Tag="201" IconVisible="False" LeftMargin="127.0075" RightMargin="253.9925" TopMargin="322.0234" BottomMargin="42.9766" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="8" Scale9Height="4" ctype="ImageViewObjectData">
                    <Size X="32.0000" Y="44.0000" />
                    <AnchorPoint />
                    <Position X="127.0075" Y="42.9766" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3075" Y="0.1051" />
                    <PreSize X="0.0775" Y="0.1076" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_3.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_6" ActionTag="309348394" CallBackType="Touch" CallBackName="onTouchObject" Tag="202" IconVisible="False" LeftMargin="269.6276" RightMargin="107.3724" TopMargin="157.0740" BottomMargin="208.9260" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="12" Scale9Height="3" ctype="ImageViewObjectData">
                    <Size X="36.0000" Y="43.0000" />
                    <AnchorPoint />
                    <Position X="269.6276" Y="208.9260" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6529" Y="0.5108" />
                    <PreSize X="0.0872" Y="0.1051" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_4.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_7" ActionTag="-498668941" CallBackType="Touch" CallBackName="onTouchObject" Tag="203" IconVisible="False" LeftMargin="229.4581" RightMargin="145.5419" TopMargin="230.3259" BottomMargin="125.6741" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="14" Scale9Height="13" ctype="ImageViewObjectData">
                    <Size X="38.0000" Y="53.0000" />
                    <AnchorPoint />
                    <Position X="229.4581" Y="125.6741" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5556" Y="0.3073" />
                    <PreSize X="0.0920" Y="0.1296" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_5.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_7_0" ActionTag="-915279635" CallBackType="Touch" CallBackName="onTouchObject" Tag="204" IconVisible="False" LeftMargin="299.5790" RightMargin="83.4210" TopMargin="262.2483" BottomMargin="100.7517" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="6" Scale9Height="6" ctype="ImageViewObjectData">
                    <Size X="30.0000" Y="46.0000" />
                    <AnchorPoint />
                    <Position X="299.5790" Y="100.7517" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7254" Y="0.2463" />
                    <PreSize X="0.0726" Y="0.1125" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_6.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_7_1" ActionTag="-701224326" CallBackType="Touch" CallBackName="onTouchObject" Tag="205" IconVisible="False" LeftMargin="250.3046" RightMargin="130.6954" TopMargin="321.7346" BottomMargin="43.2654" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="8" Scale9Height="4" ctype="ImageViewObjectData">
                    <Size X="32.0000" Y="44.0000" />
                    <AnchorPoint />
                    <Position X="250.3046" Y="43.2654" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6061" Y="0.1058" />
                    <PreSize X="0.0775" Y="0.1076" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_7.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_2" ActionTag="-1831203460" CallBackType="Touch" CallBackName="onTouchObject" Tag="198" IconVisible="False" LeftMargin="165.1282" RightMargin="165.8718" TopMargin="47.8499" BottomMargin="325.1501" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="11" BottomEage="11" Scale9OriginX="12" Scale9OriginY="11" Scale9Width="58" Scale9Height="14" ctype="ImageViewObjectData">
                    <Size X="82.0000" Y="36.0000" />
                    <AnchorPoint />
                    <Position X="165.1282" Y="325.1501" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3998" Y="0.7950" />
                    <PreSize X="0.1985" Y="0.0880" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_17.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0_1" ActionTag="-351286143" CallBackType="Touch" CallBackName="onTouchObject" Tag="197" IconVisible="False" LeftMargin="133.4616" RightMargin="133.5384" TopMargin="81.3898" BottomMargin="276.6102" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="122" Scale9Height="11" ctype="ImageViewObjectData">
                    <Size X="146.0000" Y="51.0000" />
                    <AnchorPoint />
                    <Position X="133.4616" Y="276.6102" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3232" Y="0.6763" />
                    <PreSize X="0.3535" Y="0.1247" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_16.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_vien" CanEdit="False" ActionTag="2131951706" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="383" Scale9Height="379" ctype="ImageViewObjectData">
                    <Size X="413.0000" Y="409.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="206.5000" Y="204.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/ToMauImg/THVCT_20.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="303.5684" Y="173.9718" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2965" Y="0.2265" />
                <PreSize X="0.4033" Y="0.5326" />
                <SingleColor A="255" R="26" G="26" B="26" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>