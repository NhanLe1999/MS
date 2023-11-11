<GameFile>
  <PropertyGroup Name="MJUpdatePopup" Type="Layer" ID="8698b3ef-6832-40b0-ace9-3fc980b89498" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="90" Speed="1.0000" ActivedAnimationName="hide">
        <Timeline ActionTag="856990643" Property="Alpha">
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
        <Timeline ActionTag="1153921209" Property="Scale">
          <ScaleFrame FrameIndex="0" X="0.0100" Y="0.0100">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="30" X="1.0000" Y="1.0000">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="1.0000" Y="1.0000">
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
      <ObjectData Name="layer" CustomClassName="MJUpdatePopup" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="layout" ActionTag="856990643" Alpha="34" Tag="14" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TopMargin="1.7625" BottomMargin="-1.7625" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position Y="-1.7625" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition Y="-0.0023" />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="0" G="0" B="0" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="alert_layout" ActionTag="1153921209" Tag="25" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="310.5000" RightMargin="310.5000" TopMargin="185.0000" BottomMargin="185.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="403.0000" Y="398.0000" />
            <Children>
              <AbstractNodeData Name="alert_bg" ActionTag="521911233" Tag="105" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-217.7268" RightMargin="-221.2732" TopMargin="-62.3332" BottomMargin="-51.6668" LeftEage="152" RightEage="152" TopEage="171" BottomEage="171" Scale9OriginX="152" Scale9OriginY="171" Scale9Width="538" Scale9Height="170" ctype="ImageViewObjectData">
                <Size X="842.0000" Y="512.0000" />
                <Children>
                  <AbstractNodeData Name="label_no" ActionTag="1188828790" CallBackType="Click" CallBackName="onCancel" Tag="51" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" LeftMargin="613.1865" RightMargin="108.8285" TopMargin="438.7188" BottomMargin="23.2588" TouchEnable="True" FontSize="20" ButtonText="skip" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="259" Scale9Height="28" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="119.9850" Y="50.0224" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="673.1790" Y="48.2700" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.7995" Y="0.0943" />
                    <PreSize X="0.1425" Y="0.0977" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                    <TextColor A="255" R="255" G="255" B="255" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="update_popup/update_bt.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="label_title" ActionTag="1665880079" Tag="88" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="11.0000" RightMargin="11.0000" TopMargin="14.4400" BottomMargin="413.5600" IsCustomSize="True" FontSize="30" LabelText="Cập nhật phiên bản mới 3.0.3" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="820.0000" Y="84.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="421.0000" Y="455.5600" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="133" B="0" />
                    <PrePosition X="0.5000" Y="0.8898" />
                    <PreSize X="0.9739" Y="0.1641" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="btok" ActionTag="1445712659" CallBackType="Click" CallBackName="onOk" Tag="7" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="276.5000" RightMargin="276.5000" TopMargin="438.8085" BottomMargin="23.1915" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="8" RightEage="8" TopEage="11" BottomEage="11" Scale9OriginX="8" Scale9OriginY="11" Scale9Width="273" Scale9Height="28" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="289.0000" Y="50.0000" />
                    <Children>
                      <AbstractNodeData Name="label_yes" ActionTag="-2029931772" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="45.0000" RightMargin="45.0000" TopMargin="10.0000" BottomMargin="10.0000" FontSize="25" LabelText="Cập nhật ngay!" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="199.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="144.5000" Y="25.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6886" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="421.0000" Y="48.1915" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.0941" />
                    <PreSize X="0.3432" Y="0.0977" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <NormalFileData Type="Normal" Path="update_popup/update_bt.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="203.2732" Y="204.3332" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5044" Y="0.5134" />
                <PreSize X="2.0893" Y="1.2864" />
                <FileData Type="Normal" Path="update_popup/update_bg.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="scrollview" ActionTag="545898921" Tag="11" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="-183.5000" RightMargin="-183.5000" TopMargin="44.0000" BottomMargin="44.0000" TouchEnable="True" ClipAble="True" BackColorAlpha="59" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Vertical" ctype="ScrollViewObjectData">
                <Size X="770.0000" Y="310.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="201.5000" Y="199.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.9107" Y="0.7789" />
                <SingleColor A="255" R="255" G="150" B="100" />
                <FirstColor A="255" R="255" G="150" B="100" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
                <InnerNodeSize Width="770" Height="310" />
              </AbstractNodeData>
              <AbstractNodeData Name="label_message" ActionTag="100557992" VisibleForFrame="False" Tag="6" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="-147.8955" RightMargin="-149.1045" TopMargin="45.4963" BottomMargin="52.5037" IsCustomSize="True" FontSize="23" LabelText="- Bổ sung thêm cách dạy nội dung Phonics và Sight words trong bài học&#xA;- Thêm mới các khoá học tiếng Nga, tiếng Pháp, tiếng Trung, tiếng Việt và tiếng Tây Ban Nha&#xA;- Nâng cấp nội dung các khoá học tiếng Anh Mỹ&#xA;&#xA;Lưu ý: Khi bạn cập nhật lên phiên bản mới này, tất cả nội dung học của bạn đã tải sẽ bị xoá. Bạn cần tải lại nội dung mới. Các stickers của bé sẽ được lưu giữ.  " ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="700.0000" Y="300.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="202.1045" Y="202.5037" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="26" G="26" B="26" />
                <PrePosition X="0.5015" Y="0.5088" />
                <PreSize X="1.7370" Y="0.7538" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="0.6221" ScaleY="0.6221" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
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