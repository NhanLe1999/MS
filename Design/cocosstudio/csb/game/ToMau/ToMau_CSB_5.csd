<GameFile>
  <PropertyGroup Name="ToMau_CSB" Type="Layer" ID="d247043a-037f-4275-89a4-4cfcaf9dd3fb" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="ToMau" Tag="79" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="bg_1" ActionTag="-795817293" Tag="81" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-336.6688" RightMargin="-339.3312" TopMargin="1.3056" BottomMargin="-1.3056" ctype="SpriteObjectData">
            <Size X="1700.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="513.3312" Y="382.6944" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5013" Y="0.4983" />
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
              <AbstractNodeData Name="board" ActionTag="-2038146564" Tag="91" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="86.5000" RightMargin="86.5000" TopMargin="138.0000" BottomMargin="138.0000" LeftEage="274" RightEage="274" TopEage="135" BottomEage="135" Scale9OriginX="274" Scale9OriginY="135" Scale9Width="303" Scale9Height="222" ctype="ImageViewObjectData">
                <Size X="851.0000" Y="492.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.8311" Y="0.6406" />
                <FileData Type="Normal" Path="games/ToMauImg/muaxuan_Layer 1.png" Plist="" />
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
              <AbstractNodeData Name="Panel_2" ActionTag="1678840198" CallBackName="onTouchPanel" Tag="20" IconVisible="False" LeftMargin="147.2812" RightMargin="149.7188" TopMargin="168.9908" BottomMargin="173.0092" ClipAble="False" BackColorAlpha="162" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="727.0000" Y="426.0000" />
                <Children>
                  <AbstractNodeData Name="Image_6_0" ActionTag="-549386236" CallBackType="Touch" CallBackName="onTouchObject" Tag="63" IconVisible="False" LeftMargin="149.8769" RightMargin="329.1231" TopMargin="1.6930" BottomMargin="251.3070" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="224" Scale9Height="133" ctype="ImageViewObjectData">
                    <Size X="248.0000" Y="173.0000" />
                    <AnchorPoint />
                    <Position X="149.8769" Y="251.3070" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2062" Y="0.5899" />
                    <PreSize X="0.3411" Y="0.4061" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_bird.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6" ActionTag="-356835940" CallBackType="Touch" CallBackName="onTouchObject" Tag="19" IconVisible="False" LeftMargin="70.2977" RightMargin="2.7023" TopMargin="235.7606" BottomMargin="11.2394" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="630" Scale9Height="139" ctype="ImageViewObjectData">
                    <Size X="654.0000" Y="179.0000" />
                    <AnchorPoint />
                    <Position X="70.2977" Y="11.2394" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0967" Y="0.0264" />
                    <PreSize X="0.8996" Y="0.4202" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_canhcay.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_3" ActionTag="1382762100" CallBackType="Touch" CallBackName="onTouchObject" Tag="16" IconVisible="False" LeftMargin="2.6441" RightMargin="638.3559" TopMargin="257.1278" BottomMargin="81.8722" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="56" Scale9Height="57" ctype="ImageViewObjectData">
                    <Size X="86.0000" Y="87.0000" />
                    <AnchorPoint />
                    <Position X="2.6441" Y="81.8722" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0036" Y="0.1922" />
                    <PreSize X="0.1183" Y="0.2042" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_flower1.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_1" ActionTag="778456474" CallBackType="Touch" CallBackName="onTouchObject" Tag="14" IconVisible="False" LeftMargin="446.1305" RightMargin="163.8695" TopMargin="132.1342" BottomMargin="175.8658" TouchEnable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="87" Scale9Height="88" ctype="ImageViewObjectData">
                    <Size X="117.0000" Y="118.0000" />
                    <AnchorPoint />
                    <Position X="446.1305" Y="175.8658" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.6137" Y="0.4128" />
                    <PreSize X="0.1609" Y="0.2770" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_flower2.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_4" ActionTag="-1143914025" CallBackType="Touch" CallBackName="onTouchObject" Tag="17" IconVisible="False" LeftMargin="276.8335" RightMargin="338.1665" TopMargin="203.3197" BottomMargin="109.6803" TouchEnable="True" LeftEage="7" RightEage="7" TopEage="8" BottomEage="8" Scale9OriginX="7" Scale9OriginY="8" Scale9Width="98" Scale9Height="97" ctype="ImageViewObjectData">
                    <Size X="112.0000" Y="113.0000" />
                    <AnchorPoint />
                    <Position X="276.8335" Y="109.6803" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3808" Y="0.2575" />
                    <PreSize X="0.1541" Y="0.2653" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_flower3.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_6_0_0" ActionTag="1490338426" CallBackType="Touch" CallBackName="onTouchObject" Tag="80" IconVisible="False" LeftMargin="95.5256" RightMargin="454.4744" TopMargin="189.5496" BottomMargin="27.4504" TouchEnable="True" LeftEage="12" RightEage="12" TopEage="20" BottomEage="20" Scale9OriginX="12" Scale9OriginY="20" Scale9Width="153" Scale9Height="169" ctype="ImageViewObjectData">
                    <Size X="177.0000" Y="209.0000" />
                    <AnchorPoint />
                    <Position X="95.5256" Y="27.4504" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1314" Y="0.0644" />
                    <PreSize X="0.2435" Y="0.4906" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_leaves.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_vien" CanEdit="False" ActionTag="2131951706" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="697" Scale9Height="396" ctype="ImageViewObjectData">
                    <Size X="727.0000" Y="426.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="363.5000" Y="213.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <FileData Type="Normal" Path="games/ToMauImg/muaxuan_line.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint />
                <Position X="147.2812" Y="173.0092" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.1438" Y="0.2253" />
                <PreSize X="0.7100" Y="0.5547" />
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