<GameFile>
  <PropertyGroup Name="EndTrialLessonPopup" Type="Layer" ID="62b7b0b0-16ac-4265-a664-d2f39d7b73ab" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="EndTrialLessonPopup" Tag="108" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" ActionTag="-2054259780" Tag="890" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="127" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="popup_bg" ActionTag="323242473" Tag="109" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="169.7200" RightMargin="169.7200" TopMargin="189.1650" BottomMargin="189.1650" Scale9Enable="True" LeftEage="199" RightEage="172" TopEage="156" BottomEage="133" Scale9OriginX="199" Scale9OriginY="156" Scale9Width="315" Scale9Height="102" ctype="ImageViewObjectData">
                <Size X="684.5600" Y="389.6700" />
                <Children>
                  <AbstractNodeData Name="lb.f2p.title.of.end.trial.lesson.popup" ActionTag="2024683639" Tag="113" IconVisible="False" LeftMargin="163.7800" RightMargin="163.7800" TopMargin="28.7100" BottomMargin="320.9600" FontSize="33" LabelText="Học thử đã kết thúc" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="357.0000" Y="40.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.2800" Y="340.9600" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="1" G="196" B="248" />
                    <PrePosition X="0.5000" Y="0.8750" />
                    <PreSize X="0.5215" Y="0.1027" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_line" ActionTag="517482216" Tag="222" IconVisible="False" LeftMargin="11.8550" RightMargin="11.8550" TopMargin="73.7450" BottomMargin="298.2750" LeftEage="218" RightEage="218" TopEage="5" BottomEage="5" Scale9OriginX="218" Scale9OriginY="5" Scale9Width="226" Scale9Height="8" ctype="ImageViewObjectData">
                    <Size X="660.8500" Y="17.6500" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.2800" Y="307.1000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.7881" />
                    <PreSize X="0.9654" Y="0.0453" />
                    <FileData Type="Normal" Path="free2paid/popup/line.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="lb.f2p.content.of.end.trial.lesson.popup" ActionTag="2089902294" Tag="149" IconVisible="False" LeftMargin="38.5400" RightMargin="38.5400" TopMargin="112.1500" BottomMargin="168.0200" IsCustomSize="True" FontSize="24" LabelText="Để bé có thể học tiếp, ba mẹ vui lòng mở khóa toàn bộ nội dung, hoặc đăng ký miễn phí để tiếp tục trải nghiệm." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="607.4800" Y="109.5000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="342.2800" Y="222.7700" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="132" G="128" B="132" />
                    <PrePosition X="0.5000" Y="0.5717" />
                    <PreSize X="0.8874" Y="0.2810" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button-left" ActionTag="163094751" CallBackType="Click" CallBackName="onClickLeftButton" Tag="223" IconVisible="False" LeftMargin="54.0750" RightMargin="355.9750" TopMargin="272.5350" BottomMargin="23.1450" TouchEnable="True" FontSize="24" Scale9Enable="True" LeftEage="51" RightEage="51" TopEage="39" BottomEage="39" Scale9OriginX="51" Scale9OriginY="39" Scale9Width="55" Scale9Height="16" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="274.5100" Y="93.9900" />
                    <Children>
                      <AbstractNodeData Name="lb.f2p.text.of.button.unclock.all.lesson" ActionTag="2123959187" Tag="225" IconVisible="False" LeftMargin="27.7600" RightMargin="27.7500" TopMargin="22.4900" BottomMargin="42.5000" FontSize="24" LabelText="Mở khóa toàn bộ" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="219.0000" Y="29.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="137.2600" Y="57.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.6064" />
                        <PreSize X="0.7978" Y="0.3085" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="191.3300" Y="70.1400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2795" Y="0.1800" />
                    <PreSize X="0.4010" Y="0.2412" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <NormalFileData Type="Normal" Path="free2paid/buttons/button2.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button-right" ActionTag="-1917287937" CallBackType="Click" CallBackName="onClickRightButton" Tag="224" IconVisible="False" LeftMargin="355.6250" RightMargin="54.4250" TopMargin="272.5350" BottomMargin="23.1450" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="51" RightEage="51" TopEage="38" BottomEage="32" Scale9OriginX="51" Scale9OriginY="38" Scale9Width="55" Scale9Height="24" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="274.5100" Y="93.9900" />
                    <Children>
                      <AbstractNodeData Name="lb.f2p.text.of.button.signup.free" ActionTag="1372335162" Tag="226" IconVisible="False" LeftMargin="21.1561" RightMargin="20.3539" TopMargin="22.4900" BottomMargin="42.5000" FontSize="24" LabelText="Đăng ký miễn phí" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="233.0000" Y="29.0000" />
                        <AnchorPoint ScaleX="0.4983" ScaleY="0.5000" />
                        <Position X="137.2600" Y="57.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="1" G="196" B="248" />
                        <PrePosition X="0.5000" Y="0.6064" />
                        <PreSize X="0.8488" Y="0.3085" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-ExtraBold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="492.8800" Y="70.1400" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7200" Y="0.1800" />
                    <PreSize X="0.4010" Y="0.2412" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <NormalFileData Type="Normal" Path="free2paid/buttons/button1.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6685" Y="0.5074" />
                <FileData Type="Normal" Path="free2paid/popup/popup_background.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>