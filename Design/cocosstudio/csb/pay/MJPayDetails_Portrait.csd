<GameFile>
  <PropertyGroup Name="MJPayDetails_Portrait" Type="Layer" ID="7d01ec09-cc49-4a6d-b612-32925205ac86" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="65" Speed="1.0000" ActivedAnimationName="show">
        <Timeline ActionTag="976033210" Property="Scale">
          <ScaleFrame FrameIndex="5" X="0.2500" Y="0.2500">
            <EasingData Type="26" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="25" X="1.0000" Y="1.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="40" X="1.0000" Y="1.0000">
            <EasingData Type="25" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="60" X="0.2500" Y="0.2500">
            <EasingData Type="26" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="976033210" Property="Alpha">
          <IntFrame FrameIndex="5" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="25" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="40" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="60" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
        <Timeline ActionTag="689370493" Property="RotationSkew">
          <ScaleFrame FrameIndex="30" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="35" X="0.0000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="689370493" Property="Alpha">
          <IntFrame FrameIndex="0" Value="0">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="30" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="35" Value="255">
            <EasingData Type="0" />
          </IntFrame>
          <IntFrame FrameIndex="65" Value="0">
            <EasingData Type="0" />
          </IntFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="show" StartIndex="0" EndIndex="30">
          <RenderColor A="255" R="135" G="206" B="250" />
        </AnimationInfo>
        <AnimationInfo Name="hide" StartIndex="35" EndIndex="65">
          <RenderColor A="255" R="255" G="255" B="224" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Layer" CustomClassName="MJPayDetails" Tag="74" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="content" ActionTag="689370493" CallBackType="Click" CallBackName="onBack" Tag="75" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="190" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="img" ActionTag="976033210" Tag="76" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="84.0000" BottomMargin="84.0000" TouchEnable="True" Scale9Enable="True" LeftEage="16" RightEage="16" TopEage="16" BottomEage="16" Scale9OriginX="16" Scale9OriginY="16" Scale9Width="18" Scale9Height="18" ctype="ImageViewObjectData">
                <Size X="700.0000" Y="600.0000" />
                <Children>
                  <AbstractNodeData Name="text_title" ActionTag="669980383" VisibleForFrame="False" Tag="197" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-10.5000" RightMargin="589.5000" TopMargin="286.5000" BottomMargin="286.5000" FontSize="20" LabelText="Please Click" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="121.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="50.0000" Y="300.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="25" G="166" B="212" />
                    <PrePosition X="0.0714" Y="0.5000" />
                    <PreSize X="0.1729" Y="0.0450" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                    <OutlineColor A="255" R="28" G="112" B="194" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="button_continue" ActionTag="-142628684" CallBackType="Click" CallBackName="onContinue" Tag="37" RotationSkewX="-90.0000" RotationSkewY="-90.0000" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="380.9800" RightMargin="-334.9800" TopMargin="208.0200" BottomMargin="301.9800" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="494" Scale9Height="68" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="654.0000" Y="90.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="1110970632" Tag="38" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="250.5000" RightMargin="250.5000" TopMargin="25.0000" BottomMargin="25.0000" FontSize="28" LabelText="CONTINUE" OutlineEnabled="True" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="153.0000" Y="40.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="327.0000" Y="45.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.2339" Y="0.4444" />
                        <FontResource Type="Normal" Path="fonts/monkeystories_ultra.ttf" Plist="" />
                        <OutlineColor A="255" R="28" G="112" B="194" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" />
                    <Position X="707.9800" Y="301.9800" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="1.0114" Y="0.5033" />
                    <PreSize X="0.9343" Y="0.1500" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Normal" Path="mjstory/popup/popup_welcome_bt2.png" Plist="" />
                    <NormalFileData Type="Normal" Path="mjstory/popup/popup_btok.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Button_7" ActionTag="2072473866" CallBackType="Click" CallBackName="onBack" Tag="438" IconVisible="False" PositionPercentYEnabled="True" LeftMargin="-50.0000" RightMargin="650.0000" TopMargin="-50.0000" BottomMargin="550.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="6" BottomEage="6" Scale9OriginX="-15" Scale9OriginY="-6" Scale9Width="30" Scale9Height="12" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                    <Size X="100.0000" Y="100.0000" />
                    <Children>
                      <AbstractNodeData Name="Image_11" ActionTag="1824527410" Tag="437" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="12.5000" RightMargin="12.5000" TopMargin="12.5000" BottomMargin="12.5000" LeftEage="11" RightEage="11" TopEage="12" BottomEage="12" Scale9OriginX="11" Scale9OriginY="12" Scale9Width="53" Scale9Height="51" ctype="ImageViewObjectData">
                        <Size X="75.0000" Y="75.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="50.0000" Y="50.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.7500" Y="0.7500" />
                        <FileData Type="Normal" Path="mjstory/papgemenucontrol_btclose.png" Plist="" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position Y="600.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="1.0000" />
                    <PreSize X="0.1429" Y="0.1667" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="listview" ActionTag="1569669099" Tag="228" IconVisible="False" LeftMargin="325.0000" RightMargin="-175.0000" TopMargin="-300.0000" BottomMargin="300.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ScrollDirectionType="0" ctype="ListViewObjectData">
                    <Size X="550.0000" Y="600.0000" />
                    <AnchorPoint />
                    <Position X="325.0000" Y="300.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4643" Y="0.5000" />
                    <PreSize X="0.7857" Y="1.0000" />
                    <SingleColor A="255" R="150" G="150" B="255" />
                    <FirstColor A="255" R="150" G="150" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="scrollview" ActionTag="2116280943" Tag="77" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BothEdge" LeftMargin="50.0000" RightMargin="100.0000" TouchEnable="True" StretchWidthEnable="True" StretchHeightEnable="True" ClipAble="True" BackColorAlpha="102" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="Horizontal" ctype="ScrollViewObjectData">
                    <Size X="550.0000" Y="600.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="325.0000" Y="300.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.4643" Y="0.5000" />
                    <PreSize X="0.7857" Y="1.0000" />
                    <SingleColor A="255" R="255" G="150" B="100" />
                    <FirstColor A="255" R="255" G="150" B="100" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                    <InnerNodeSize Width="1100" Height="600" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="text_details" ActionTag="2014460019" Tag="78" IconVisible="False" LeftMargin="1151.8792" RightMargin="-1051.8792" TopMargin="-413.4459" BottomMargin="-86.5541" IsCustomSize="True" FontSize="19" LabelText="Includes a 30 day free trial and is just $11.99 month after that&#xA;&#xA;Payment will be charged to iTunes Account at confirmation of purchase&#xA;&#xA;Subscription automatically renews unless auto-renew is turn off at least 24-hours before the end of the current period&#xA;&#xA;Account will be charged $11.99 for renewal within 24-hours prior to the end of the current period&#xA;&#xA;The user may manage subscriptions &amp; may turn off auto-renewal by going to Account Settings after purchase&#xA;&#xA;No cancellation of the current subscription is allowed during active subscription period&#xA;&#xA;Any unused portion of a free trial period will be forfeited when the user purchases a subscription to Monkey Stories!&#xA;&#xA;Privacy Policy&#xA;http://monkeystories.net&#xA;&#xA;Terms of Service&#xA;http://monkeystories.net" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="600.0000" Y="1100.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="1451.8792" Y="463.4459" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="127" G="127" B="127" />
                    <PrePosition X="2.0741" Y="0.7724" />
                    <PreSize X="0.8571" Y="1.8333" />
                    <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="0.6836" Y="0.7813" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
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