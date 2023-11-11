<GameFile>
  <PropertyGroup Name="APEnterEmailPopup" Type="Layer" ID="fec0343b-5344-4054-b0cb-0906cd04d5ca" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="974546587" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="204">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="90" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="794197901" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="10" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1153921209" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.1000" Y="1.1000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.1000" Y="1.1000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="90" X="0.0100" Y="0.0100">
            <EasingData Type="25" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="35">
          <RenderColor A="255" R="255" G="240" B="245" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="60" EndIndex="95">
          <RenderColor A="255" R="106" G="90" B="205" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="layer" CustomClassName="APEnterEmailPopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="974546587" Alpha="0" Tag="58" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" FlipY="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="281" Scale9Height="66" ctype="ImageViewObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="-1.0000" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.0000" Y="1.0000" />
            <FileData Type="Normal" Path="mjstory/explore/mjsh_explore_bar.png" Plist="" />
          </AbstractNodeData>
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="239.9300" BottomMargin="130.0700" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-99.5000" RightMargin="-99.5000" TopMargin="-145.9888" BottomMargin="-46.0112" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="298" Scale9Height="248" ctype="ImageViewObjectData">
                <Size X="602.0000" Y="590.0000" />
                <Children>
                  <AbstractNodeData Name="textfield_email_bg" ActionTag="1705708819" Tag="717" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="58.0000" RightMargin="58.0000" TopMargin="408.5000" BottomMargin="126.5000" TouchEnable="True" LeftEage="160" RightEage="160" TopEage="18" BottomEage="18" Scale9OriginX="160" Scale9OriginY="18" Scale9Width="166" Scale9Height="19" ctype="ImageViewObjectData">
                    <Size X="486.0000" Y="55.0000" />
                    <Children>
                      <AbstractNodeData Name="textfield_email_text" ActionTag="1709879218" Tag="716" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="4.8600" RightMargin="4.8600" TopMargin="11.0000" BottomMargin="11.0000" TouchEnable="True" FontSize="24" IsCustomSize="True" LabelText="" PlaceHolderText="Enter your email here" MaxLengthText="10" ctype="TextFieldObjectData">
                        <Size X="476.2800" Y="33.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="243.0000" Y="27.5000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9800" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="301.0000" Y="154.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.2610" />
                    <PreSize X="0.8073" Y="0.0932" />
                    <FileData Type="Normal" Path="mjstory/popup/textfield_email.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_message" ActionTag="100557992" Tag="6" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="30.1000" RightMargin="30.1000" TopMargin="200.0000" BottomMargin="190.0000" IsCustomSize="True" FontSize="24" LabelText="Monkey gửi tặng ba mẹ ebook &quot;Learning English effectively with Monkey Stories&quot;.&#xA;Ba mẹ vui lòng kiểm tra Email để download tài liệu đồng hành cùng con nhé." HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="541.8000" Y="200.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="301.0000" Y="290.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.5000" Y="0.4915" />
                    <PreSize X="0.9000" Y="0.3390" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="img_notice" ActionTag="1366760363" VisibleForFrame="False" Tag="736" IconVisible="False" LeftMargin="48.0000" RightMargin="500.0000" TopMargin="455.5000" BottomMargin="85.5000" LeftEage="17" RightEage="17" TopEage="16" BottomEage="16" Scale9OriginX="17" Scale9OriginY="16" Scale9Width="20" Scale9Height="17" ctype="ImageViewObjectData">
                    <Size X="54.0000" Y="49.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="75.0000" Y="110.0000" />
                    <Scale ScaleX="0.6400" ScaleY="0.6400" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.1246" Y="0.1864" />
                    <PreSize X="0.0897" Y="0.0831" />
                    <FileData Type="Normal" Path="mjstory/popup/info_popup_warning.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_notice" ActionTag="-1340544105" VisibleForFrame="False" Tag="735" IconVisible="False" LeftMargin="90.0000" RightMargin="42.0000" TopMargin="470.0000" BottomMargin="56.0000" IsCustomSize="True" FontSize="18" LabelText="Email không hợp lệ!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="470.0000" Y="64.0000" />
                    <AnchorPoint ScaleY="1.0000" />
                    <Position X="90.0000" Y="120.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="236" G="29" B="39" />
                    <PrePosition X="0.1495" Y="0.2034" />
                    <PreSize X="0.7807" Y="0.1085" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_email" ActionTag="479943864" Tag="715" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="60.2000" RightMargin="439.8000" TopMargin="375.5000" BottomMargin="185.5000" FontSize="24" LabelText="Email (*)" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="102.0000" Y="29.0000" />
                    <AnchorPoint ScaleY="0.5000" />
                    <Position X="60.2000" Y="200.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <PrePosition X="0.1000" Y="0.3390" />
                    <PreSize X="0.1694" Y="0.0492" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bt_ok" ActionTag="1445712659" CallBackType="Click" CallBackName="onOk" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="200.5000" RightMargin="200.5000" TopMargin="492.5000" BottomMargin="22.5000" TouchEnable="True" FontSize="30" ButtonText="Lưu" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="171" Scale9Height="53" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="201.0000" Y="75.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="301.0000" Y="60.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.1017" />
                    <PreSize X="0.3339" Y="0.1271" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Normal" Path="mjstory/popup/popup_bt_save_blank.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/popup_bt_save.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/popup_bt_save.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="bt_cancel" ActionTag="794197901" CallBackType="Click" CallBackName="onCancel" Tag="35" IconVisible="False" HorizontalEdge="RightEdge" VerticalEdge="TopEdge" LeftMargin="544.7393" RightMargin="-30.7393" TopMargin="134.2436" BottomMargin="367.7564" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="-15" Scale9OriginY="-11" Scale9Width="30" Scale9Height="22" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="88.0000" Y="88.0000" />
                    <Children>
                      <AbstractNodeData Name="icon" ActionTag="-1899721670" Tag="507" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="20.5000" RightMargin="20.5000" TopMargin="20.5000" BottomMargin="20.5000" LeftEage="15" RightEage="15" TopEage="15" BottomEage="15" Scale9OriginX="15" Scale9OriginY="15" Scale9Width="17" Scale9Height="17" ctype="ImageViewObjectData">
                        <Size X="47.0000" Y="47.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="44.0000" Y="44.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.5341" Y="0.5341" />
                        <FileData Type="Normal" Path="mjstory/popup/popup_ratebuy_btclose.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="588.7393" Y="411.7564" />
                    <Scale ScaleX="0.0100" ScaleY="0.0100" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.9780" Y="0.6979" />
                    <PreSize X="0.1462" Y="0.1492" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="248.9888" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6256" />
                <PreSize X="1.4938" Y="1.4824" />
                <FileData Type="Normal" Path="mjstory/popup/popup_bg_2.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="329.0700" />
            <Scale ScaleX="0.0100" ScaleY="0.0100" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.4285" />
            <PreSize X="0.3936" Y="0.5182" />
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