<GameFile>
  <PropertyGroup Name="AISpeakingClosingPopup" Type="Layer" ID="2919a81a-dbcf-4d02-bc87-53217ab79d3b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="AISpeakingClosingPopup" Tag="81" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-491726018" Alpha="127" Tag="97" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="first_call_layout" Visible="False" ActionTag="-2046287800" Tag="14" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="background" ActionTag="-1972808278" Alpha="249" Tag="60" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="256.0000" RightMargin="256.0000" TopMargin="248.0000" BottomMargin="248.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="71" Scale9Height="61" ctype="ImageViewObjectData">
                <Size X="512.0000" Y="272.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.5000" Y="0.3542" />
                <FileData Type="Normal" Path="games/ai_speaking/popup_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb.sure.to.hang.up" ActionTag="-999555840" Tag="114" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="359.0000" RightMargin="359.0000" TopMargin="341.5934" BottomMargin="391.4066" FontSize="26" LabelText="Are you sure to hang up?" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="306.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="408.9066" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="1" G="196" B="248" />
                <PrePosition X="0.5000" Y="0.5324" />
                <PreSize X="0.2988" Y="0.0456" />
                <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="love_icon" ActionTag="-879529898" Tag="17" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="443.0000" RightMargin="443.0000" TopMargin="171.9063" BottomMargin="458.0937" LeftEage="13" RightEage="13" TopEage="13" BottomEage="13" Scale9OriginX="13" Scale9OriginY="13" Scale9Width="112" Scale9Height="112" ctype="ImageViewObjectData">
                <Size X="138.0000" Y="138.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="527.0937" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6863" />
                <PreSize X="0.1348" Y="0.1797" />
                <FileData Type="Normal" Path="games/ai_speaking/closing_popup/crying_face.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="continue_button" ActionTag="-1977058853" CallBackType="Click" CallBackName="onContinueButtonClicked" Tag="110" IconVisible="False" LeftMargin="295.0392" RightMargin="516.9608" TopMargin="413.9713" BottomMargin="280.0287" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="182" Scale9Height="52" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="212.0000" Y="74.0000" />
                <Children>
                  <AbstractNodeData Name="lb.continue" ActionTag="1856455792" Tag="111" IconVisible="False" LeftMargin="76.5253" RightMargin="47.4747" TopMargin="19.4674" BottomMargin="27.5326" FontSize="20" LabelText="Continue" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="88.0000" Y="27.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="76.5253" Y="41.0326" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3610" Y="0.5545" />
                    <PreSize X="0.4151" Y="0.3649" />
                    <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="401.0392" Y="317.0287" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3916" Y="0.4128" />
                <PreSize X="0.2070" Y="0.0964" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/closing_popup/continue_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="hang_up_button" ActionTag="-359282340" CallBackType="Click" CallBackName="onHangUpClicked" Tag="112" IconVisible="False" LeftMargin="529.1640" RightMargin="284.8360" TopMargin="413.9705" BottomMargin="282.0295" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="180" Scale9Height="50" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="210.0000" Y="72.0000" />
                <Children>
                  <AbstractNodeData Name="lb.hang.up" ActionTag="-940598574" Tag="113" IconVisible="False" LeftMargin="81.5253" RightMargin="45.4747" TopMargin="17.4674" BottomMargin="27.5326" FontSize="20" LabelText="Hang up" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="83.0000" Y="27.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="81.5253" Y="41.0326" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3882" Y="0.5699" />
                    <PreSize X="0.3952" Y="0.3750" />
                    <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="634.1640" Y="318.0295" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6193" Y="0.4141" />
                <PreSize X="0.2051" Y="0.0938" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/closing_popup/hang_up_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="relearn_layout" Visible="False" ActionTag="-1751043129" Tag="168" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="background" ActionTag="-921549182" Alpha="249" Tag="169" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="184.5000" RightMargin="184.5000" TopMargin="181.5000" BottomMargin="181.5000" Scale9Enable="True" LeftEage="105" RightEage="282" TopEage="147" BottomEage="86" Scale9OriginX="105" Scale9OriginY="147" Scale9Width="268" Scale9Height="29" ctype="ImageViewObjectData">
                <Size X="655.0000" Y="405.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6396" Y="0.5273" />
                <FileData Type="Normal" Path="games/ai_speaking/closing_popup_03/bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="back_button" ActionTag="1356342819" CallBackType="Click" CallBackName="onClosePopupClicked" Tag="56" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="774.8096" RightMargin="185.1904" TopMargin="181.4707" BottomMargin="522.5293" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="64.0000" Y="64.0000" />
                <Children>
                  <AbstractNodeData Name="icon_close" ActionTag="-1501458752" Tag="57" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.0000" RightMargin="11.0000" TopMargin="11.0000" BottomMargin="11.0000" LeftEage="11" RightEage="11" TopEage="11" BottomEage="11" Scale9OriginX="11" Scale9OriginY="11" Scale9Width="20" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="42.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="32.0000" Y="32.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6563" Y="0.6563" />
                    <FileData Type="Normal" Path="games/ai_speaking/closing_popup_03/X.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="806.8096" Y="554.5293" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7879" Y="0.7220" />
                <PreSize X="0.0625" Y="0.0833" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb.conversation.paused.title" ActionTag="-1664362502" Tag="170" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="347.5000" RightMargin="347.5000" TopMargin="242.4573" BottomMargin="480.5427" FontSize="33" LabelText="Conversation Paused" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="329.0000" Y="45.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="503.0427" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="1" G="196" B="248" />
                <PrePosition X="0.5000" Y="0.6550" />
                <PreSize X="0.3213" Y="0.0586" />
                <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="exit_button" ActionTag="-1699368074" CallBackType="Click" CallBackName="onExitButtonClicked" Tag="172" IconVisible="False" LeftMargin="232.4195" RightMargin="535.5805" TopMargin="447.9097" BottomMargin="236.0903" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="47" RightEage="47" TopEage="27" BottomEage="27" Scale9OriginX="47" Scale9OriginY="27" Scale9Width="162" Scale9Height="30" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="256.0000" Y="84.0000" />
                <Children>
                  <AbstractNodeData Name="lb.exit" ActionTag="1444697730" Tag="173" IconVisible="False" LeftMargin="121.4631" RightMargin="93.5369" TopMargin="22.1859" BottomMargin="32.8141" FontSize="21" LabelText="Exit" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="41.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="121.4631" Y="47.3141" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="1" G="196" B="248" />
                    <PrePosition X="0.4745" Y="0.5633" />
                    <PreSize X="0.1602" Y="0.3452" />
                    <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="360.4195" Y="278.0903" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3520" Y="0.3621" />
                <PreSize X="0.2500" Y="0.1094" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/closing_popup_02/exit_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="skip_button" ActionTag="-1798776474" CallBackType="Click" CallBackName="onSkipButtonClicked" Tag="174" IconVisible="False" LeftMargin="535.5536" RightMargin="232.4464" TopMargin="450.8306" BottomMargin="233.1694" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="33" RightEage="30" TopEage="27" BottomEage="27" Scale9OriginX="33" Scale9OriginY="27" Scale9Width="193" Scale9Height="30" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="256.0000" Y="84.0000" />
                <Children>
                  <AbstractNodeData Name="lb.skip" ActionTag="-1919454774" Tag="175" IconVisible="False" LeftMargin="120.5626" RightMargin="91.4374" TopMargin="23.5424" BottomMargin="33.4576" FontSize="20" LabelText="Skip" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="44.0000" Y="27.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="120.5626" Y="46.9576" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4709" Y="0.5590" />
                    <PreSize X="0.1719" Y="0.3214" />
                    <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="663.5536" Y="275.1694" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6480" Y="0.3583" />
                <PreSize X="0.2500" Y="0.1094" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/closing_popup_02/skip_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb.conversation.paused.content" ActionTag="1391419873" Tag="184" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="212.0000" RightMargin="212.0000" TopMargin="290.0220" BottomMargin="317.9780" IsCustomSize="True" FontSize="20" LabelText="Bài học đang được tạm dừng.&#xA;Bé có thể bấm Skip để chuyển sang chế độ Nói chuyện &#xA;hoặc bấm Exit để thoát khỏi bài học." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="600.0000" Y="160.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="397.9780" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="108" G="108" B="108" />
                <PrePosition X="0.5000" Y="0.5182" />
                <PreSize X="0.5859" Y="0.2083" />
                <FontResource Type="Normal" Path="fonts/Nunito-SemiBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="relearn_last_turn_layout" ActionTag="1661761033" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="False" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="background" ActionTag="-1867767643" Alpha="249" Tag="59" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="184.5000" RightMargin="184.5000" TopMargin="253.0000" BottomMargin="253.0000" Scale9Enable="True" LeftEage="105" RightEage="282" TopEage="147" BottomEage="86" Scale9OriginX="105" Scale9OriginY="147" Scale9Width="268" Scale9Height="29" ctype="ImageViewObjectData">
                <Size X="655.0000" Y="262.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6396" Y="0.3411" />
                <FileData Type="Normal" Path="games/ai_speaking/closing_popup_03/bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="back_button" ActionTag="-78747559" CallBackType="Click" CallBackName="onClosePopupClicked" Tag="60" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="772.6402" RightMargin="187.3598" TopMargin="248.6260" BottomMargin="455.3740" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="64.0000" Y="64.0000" />
                <Children>
                  <AbstractNodeData Name="icon_close" ActionTag="807780003" Tag="61" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="11.0000" RightMargin="11.0000" TopMargin="11.0000" BottomMargin="11.0000" LeftEage="11" RightEage="11" TopEage="11" BottomEage="11" Scale9OriginX="11" Scale9OriginY="11" Scale9Width="20" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="42.0000" Y="42.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="32.0000" Y="32.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.6563" Y="0.6563" />
                    <FileData Type="Normal" Path="games/ai_speaking/closing_popup_03/X.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="804.6402" Y="487.3740" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7858" Y="0.6346" />
                <PreSize X="0.0625" Y="0.0833" />
                <TextColor A="255" R="65" G="65" B="70" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb.conversation.paused.title" ActionTag="911915221" Tag="62" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="336.5000" RightMargin="336.5000" TopMargin="309.9565" BottomMargin="410.0435" FontSize="35" LabelText="Conversation Paused" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="351.0000" Y="48.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="434.0435" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="1" G="196" B="248" />
                <PrePosition X="0.5000" Y="0.5652" />
                <PreSize X="0.3428" Y="0.0625" />
                <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="exit_button" ActionTag="-246457670" CallBackType="Click" CallBackName="onExitButtonClicked" Tag="63" IconVisible="False" LeftMargin="241.4088" RightMargin="242.5912" TopMargin="396.3495" BottomMargin="288.6505" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="47" RightEage="47" TopEage="27" BottomEage="27" Scale9OriginX="47" Scale9OriginY="27" Scale9Width="446" Scale9Height="29" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="540.0000" Y="83.0000" />
                <Children>
                  <AbstractNodeData Name="lb.exit" ActionTag="-771955700" Tag="64" IconVisible="False" LeftMargin="263.6198" RightMargin="235.3802" TopMargin="22.4385" BottomMargin="31.5615" FontSize="21" LabelText="Exit" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="41.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="263.6198" Y="46.0615" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4882" Y="0.5550" />
                    <PreSize X="0.0759" Y="0.3494" />
                    <FontResource Type="Normal" Path="fonts/Nunito-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="511.4088" Y="330.1505" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4994" Y="0.4299" />
                <PreSize X="0.5273" Y="0.1081" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="games/ai_speaking/closing_popup_03/exit_button2.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>