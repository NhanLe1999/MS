<GameFile>
  <PropertyGroup Name="APLoginMethodsScreen" Type="Layer" ID="c0b80e86-29e3-4e07-8a29-4bfeb6490c4b" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="APLoginMethodsScreen" Tag="31" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="background" Visible="False" ActionTag="-933352803" Tag="207" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentHeightEnable="True" PercentHeightEnabled="True" LeftMargin="-488.0000" RightMargin="-488.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="2000.0000" Y="768.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="512.0000" Y="384.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5000" Y="0.5000" />
            <PreSize X="1.9531" Y="1.0000" />
            <SingleColor A="255" R="255" G="255" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="white_background" ActionTag="-1817342593" Tag="579" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <Children>
              <AbstractNodeData Name="title" ActionTag="1896228848" Tag="580" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="148.0000" RightMargin="148.0000" TopMargin="38.5000" BottomMargin="619.5000" FontSize="45" LabelText="Tài khoản này có %d hình thức&#xA;đăng nhập:" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="728.0000" Y="110.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="674.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="44" G="184" B="233" />
                <PrePosition X="0.5000" Y="0.8783" />
                <PreSize X="0.7109" Y="0.1432" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <OutlineColor A="255" R="0" G="119" B="182" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="listview_layout" ActionTag="1366153136" CallBackName="onNextButtonClicked" Tag="506" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="164.5000" BottomMargin="164.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                <Size X="720.0000" Y="439.0000" />
                <Children>
                  <AbstractNodeData Name="item_methods_changed" Visible="False" ActionTag="-1689615729" VisibleForFrame="False" Tag="588" IconVisible="False" BottomMargin="383.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="720.0000" Y="56.0000" />
                    <Children>
                      <AbstractNodeData Name="line" ActionTag="-778973908" Tag="515" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="-1.0000" BottomMargin="55.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="720.0000" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="56.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.0357" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="1115829750" Tag="516" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="76.0000" RightMargin="76.0000" TopMargin="13.0000" BottomMargin="13.0000" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2 và 3: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="568.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="28.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.7889" Y="0.5357" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line_0" ActionTag="-505333268" Tag="517" IconVisible="False" PositionPercentXEnabled="True" TopMargin="55.0000" BottomMargin="-1.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="720.0000" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" />
                        <PreSize X="1.0000" Y="0.0357" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="360.0000" Y="411.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.9362" />
                    <PreSize X="1.0000" Y="0.1276" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="item_or" ActionTag="733269556" Tag="599" IconVisible="False" BottomMargin="409.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="720.0000" Y="30.0000" />
                    <Children>
                      <AbstractNodeData Name="or_txt" ActionTag="1172818381" ZOrder="5" Tag="586" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="126.0000" RightMargin="126.0000" FontSize="25" LabelText=" - - - - - - - - - - - - - %s - - - - - - - - - - - -" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="468.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="15.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.6500" Y="1.0000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="0" G="119" B="182" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position Y="409.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.9317" />
                    <PreSize X="1.0000" Y="0.0683" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="item" Visible="False" ActionTag="656916166" Tag="589" IconVisible="False" BottomMargin="364.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="720.0000" Y="75.0000" />
                    <Children>
                      <AbstractNodeData Name="method_name" ActionTag="193014995" Tag="509" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="36.0000" RightMargin="523.0000" BottomMargin="45.0000" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1846767724" Tag="510" IconVisible="False" LeftMargin="64.9001" RightMargin="-173.9001" TopMargin="44.7784" BottomMargin="-44.7784" FontSize="25" LabelText="Số điện thoại: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="270.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9001" Y="-29.7784" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-0.9926" />
                            <PreSize X="1.6770" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="1.0000" />
                        <Position X="36.0000" Y="75.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0500" Y="1.0000" />
                        <PreSize X="0.2236" Y="0.4000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint />
                    <Position Y="364.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition Y="0.8292" />
                    <PreSize X="1.0000" Y="0.1708" />
                    <SingleColor A="255" R="150" G="200" B="255" />
                    <FirstColor A="255" R="150" G="200" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="listview" ActionTag="991213118" Tag="518" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TouchEnable="True" ClipAble="True" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ScrollDirectionType="0" ItemMargin="20" DirectionType="Vertical" ctype="ListViewObjectData">
                    <Size X="720.0000" Y="439.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="360.0000" Y="219.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <SingleColor A="255" R="0" G="128" B="0" />
                    <FirstColor A="255" R="150" G="150" B="255" />
                    <EndColor A="255" R="255" G="255" B="255" />
                    <ColorVector ScaleY="1.0000" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="item_methods_changed2" ActionTag="-1059876646" Tag="832" IconVisible="False" TopMargin="179.2908" BottomMargin="220.7092" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                    <Size X="720.0000" Y="39.0000" />
                    <Children>
                      <AbstractNodeData Name="line" ActionTag="-1971407024" Tag="833" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" TopMargin="-1.0000" BottomMargin="38.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="720.0000" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="39.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="1.0000" />
                        <PreSize X="1.0000" Y="0.0513" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="-1238168843" Tag="834" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="76.0000" RightMargin="76.0000" TopMargin="9.0000" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2 và 3: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="568.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" />
                        <Position X="360.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" />
                        <PreSize X="0.7889" Y="0.7692" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="360.0000" Y="240.2092" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5472" />
                    <PreSize X="1.0000" Y="0.0888" />
                    <SingleColor A="255" R="150" G="200" B="255" />
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
                <PreSize X="0.7031" Y="0.5716" />
                <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Panel_1" Visible="False" ActionTag="-103213880" VisibleForFrame="False" Tag="1489" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" TopMargin="284.0000" BottomMargin="284.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="0" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                <Size X="1024.0000" Y="200.0000" />
                <Children>
                  <AbstractNodeData Name="2_method1" Visible="False" ActionTag="-927089036" Tag="1490" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="-68.0000" BottomMargin="-68.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="720.0000" Y="336.0000" />
                    <Children>
                      <AbstractNodeData Name="method_1" ActionTag="-244208271" Tag="1577" IconVisible="False" LeftMargin="33.1170" RightMargin="530.8830" TopMargin="43.6342" BottomMargin="262.3658" FontSize="25" LabelText="Hình thức 1:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="156.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1992820770" Tag="1578" IconVisible="False" LeftMargin="64.9031" RightMargin="-204.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Tên đăng nhập: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="296.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4160" Y="-1.1593" />
                            <PreSize X="1.8974" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1170" Y="277.3658" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.8255" />
                        <PreSize X="0.2167" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_2" ActionTag="875721055" Tag="1579" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="141.6706" BottomMargin="164.3294" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="1745836556" Tag="1580" IconVisible="False" LeftMargin="64.9031" RightMargin="-173.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Số điện thoại: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="270.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.6770" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="179.3294" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.5337" />
                        <PreSize X="0.2236" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line" ActionTag="1314699654" Tag="1581" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.7200" RightMargin="0.7200" TopMargin="264.9999" BottomMargin="69.0001" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="70.0001" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.2083" />
                        <PreSize X="0.9980" Y="0.0060" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="398437667" Tag="1585" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="104.5000" RightMargin="104.5000" TopMargin="285.5072" BottomMargin="20.4928" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="511.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="35.4928" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1056" />
                        <PreSize X="0.7097" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="100.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7031" Y="1.6800" />
                    <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="2_method2" Visible="False" ActionTag="-1970565063" Tag="450" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="-68.0000" BottomMargin="-68.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="720.0000" Y="336.0000" />
                    <Children>
                      <AbstractNodeData Name="method_1" ActionTag="399563943" Tag="451" IconVisible="False" LeftMargin="33.1200" RightMargin="530.8800" TopMargin="58.3023" BottomMargin="247.6977" FontSize="25" LabelText="Hình thức 1:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="156.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1117321776" Tag="452" IconVisible="False" LeftMargin="64.9031" RightMargin="-204.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Tên đăng nhập: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="296.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4160" Y="-1.1593" />
                            <PreSize X="1.8974" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="262.6977" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.7818" />
                        <PreSize X="0.2167" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_2" ActionTag="-807308015" Tag="455" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="241.5453" BottomMargin="64.4547" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="2004140287" Tag="456" IconVisible="False" LeftMargin="64.9031" RightMargin="-130.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Facebook: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="227.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.4099" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="79.4547" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.2365" />
                        <PreSize X="0.2236" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line" ActionTag="1957041037" Tag="457" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.7200" RightMargin="0.7200" TopMargin="165.0000" BottomMargin="169.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="170.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.5060" />
                        <PreSize X="0.9980" Y="0.0060" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="-333453017" Tag="458" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="114.5000" RightMargin="114.5000" TopMargin="185.5072" BottomMargin="120.4928" FontSize="25" LabelText="Mật khẩu cho hình thức 1: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="491.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="135.4928" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.4033" />
                        <PreSize X="0.6819" Y="0.0893" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line_0" ActionTag="744150394" Tag="459" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0720" RightMargin="1.3680" TopMargin="226.4396" BottomMargin="107.5604" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="359.3520" Y="108.5604" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4991" Y="0.3231" />
                        <PreSize X="0.9980" Y="0.0060" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="100.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7031" Y="1.6800" />
                    <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="3_method1" Visible="False" ActionTag="1180163237" CallBackName="onNextButtonClicked" Tag="1484" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="-81.0000" BottomMargin="-101.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="720.0000" Y="382.0000" />
                    <Children>
                      <AbstractNodeData Name="method_1" ActionTag="-530197100" Tag="1485" IconVisible="False" LeftMargin="33.1201" RightMargin="530.8799" TopMargin="30.6659" BottomMargin="321.3341" FontSize="25" LabelText="Hình thức 1:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="156.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1830685742" Tag="1486" IconVisible="False" LeftMargin="64.9031" RightMargin="-204.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Tên đăng nhập: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="296.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4160" Y="-1.1593" />
                            <PreSize X="1.8974" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1201" Y="336.3341" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.8805" />
                        <PreSize X="0.2167" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_2" ActionTag="-491961092" Tag="1487" IconVisible="False" LeftMargin="33.1201" RightMargin="525.8799" TopMargin="128.7025" BottomMargin="223.2975" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="947419999" Tag="1488" IconVisible="False" LeftMargin="64.9031" RightMargin="-173.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Số điện thoại: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="270.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.6770" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1201" Y="238.2975" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.6238" />
                        <PreSize X="0.2236" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_3" ActionTag="263148040" Tag="1489" IconVisible="False" LeftMargin="33.1201" RightMargin="525.8799" TopMargin="215.6702" BottomMargin="136.3298" FontSize="25" LabelText="Hình thức 3:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1886588449" Tag="1490" IconVisible="False" LeftMargin="64.9031" RightMargin="-476.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Email: nguyenhoangthienphuoc@gmail.com" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="573.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="3.5590" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1201" Y="151.3298" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.3962" />
                        <PreSize X="0.2236" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line" ActionTag="1079027005" Tag="1493" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.7200" RightMargin="0.7200" TopMargin="308.9996" BottomMargin="71.0004" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="72.0004" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.1885" />
                        <PreSize X="0.9980" Y="0.0052" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="-1615491034" Tag="1494" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="76.0000" RightMargin="76.0000" TopMargin="319.5071" BottomMargin="32.4929" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2 và 3: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="568.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="47.4929" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.1243" />
                        <PreSize X="0.7889" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="90.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.4500" />
                    <PreSize X="0.7031" Y="1.9100" />
                    <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="3_method2" Visible="False" ActionTag="271250092" Tag="1617" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="-91.0000" BottomMargin="-91.0000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="720.0000" Y="382.0000" />
                    <Children>
                      <AbstractNodeData Name="method_1" ActionTag="827245952" Tag="1618" IconVisible="False" LeftMargin="33.1175" RightMargin="530.8825" TopMargin="29.6339" BottomMargin="322.3661" FontSize="25" LabelText="Hình thức 1:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="156.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="81982147" Tag="1619" IconVisible="False" LeftMargin="64.9031" RightMargin="-204.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Tên đăng nhập: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="296.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4160" Y="-1.1593" />
                            <PreSize X="1.8974" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1175" Y="337.3661" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.8832" />
                        <PreSize X="0.2167" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_2" ActionTag="1268880703" Tag="1620" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="127.6705" BottomMargin="224.3295" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="2001804463" Tag="1621" IconVisible="False" LeftMargin="64.9031" RightMargin="-173.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Số điện thoại: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="270.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.6770" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="239.3295" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.6265" />
                        <PreSize X="0.2236" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_3" ActionTag="-776743914" Tag="1622" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="294.5453" BottomMargin="57.4547" FontSize="25" LabelText="Hình thức 3:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-577335311" Tag="1623" IconVisible="False" LeftMargin="64.9031" RightMargin="-130.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Facebook: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="227.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.4099" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="72.4547" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.1897" />
                        <PreSize X="0.2236" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line" ActionTag="-661104273" Tag="1624" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.7200" RightMargin="0.7200" TopMargin="231.0000" BottomMargin="149.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3927" />
                        <PreSize X="0.9980" Y="0.0052" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="-748295161" Tag="1625" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="104.5000" RightMargin="104.5000" TopMargin="241.5072" BottomMargin="110.4928" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="511.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="125.4928" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.3285" />
                        <PreSize X="0.7097" Y="0.0785" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line_0" ActionTag="-898223255" Tag="1626" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0720" RightMargin="1.3680" TopMargin="282.4396" BottomMargin="97.5604" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="359.3520" Y="98.5604" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4991" Y="0.2580" />
                        <PreSize X="0.9980" Y="0.0052" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="100.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7031" Y="1.9100" />
                    <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="4_method" Visible="False" ActionTag="1153654434" CallBackName="onNextButtonClicked" Tag="1627" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="152.0000" RightMargin="152.0000" TopMargin="-119.5000" BottomMargin="-119.5000" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="17" BottomEage="17" Scale9OriginX="15" Scale9OriginY="17" Scale9Width="18" Scale9Height="20" ctype="ImageViewObjectData">
                    <Size X="720.0000" Y="439.0000" />
                    <Children>
                      <AbstractNodeData Name="method_1" ActionTag="439346316" Tag="1628" IconVisible="False" LeftMargin="33.1200" RightMargin="530.8800" TopMargin="9.6666" BottomMargin="399.3334" FontSize="25" LabelText="Hình thức 1:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="156.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-676778796" Tag="1629" IconVisible="False" LeftMargin="64.9031" RightMargin="-204.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Tên đăng nhập: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="296.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4160" Y="-1.1593" />
                            <PreSize X="1.8974" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="414.3334" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.9438" />
                        <PreSize X="0.2167" Y="0.0683" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_2" ActionTag="402888017" Tag="1630" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="107.7031" BottomMargin="301.2969" FontSize="25" LabelText="Hình thức 2:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-1466355078" Tag="1631" IconVisible="False" LeftMargin="64.9031" RightMargin="-173.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Số điện thoại: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="270.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="1.6770" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="316.2969" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.7205" />
                        <PreSize X="0.2236" Y="0.0683" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_3" ActionTag="-1295720327" Tag="1637" IconVisible="False" LeftMargin="33.1200" RightMargin="525.8800" TopMargin="194.6705" BottomMargin="214.3295" FontSize="25" LabelText="Hình thức 3:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="161.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="1104234808" Tag="1638" IconVisible="False" LeftMargin="64.9031" RightMargin="-476.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Email: nguyenhoangthienphuoc@gmail.com" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="573.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.4031" Y="-1.1593" />
                            <PreSize X="3.5590" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="229.3295" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.5224" />
                        <PreSize X="0.2236" Y="0.0683" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="method_4" ActionTag="149865149" Tag="1632" IconVisible="False" LeftMargin="33.1200" RightMargin="523.8800" TopMargin="351.5453" BottomMargin="57.4547" FontSize="25" LabelText="Hình thức 4:" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="163.0000" Y="30.0000" />
                        <Children>
                          <AbstractNodeData Name="des_txt" ActionTag="-432855662" Tag="1633" IconVisible="False" LeftMargin="64.9031" RightMargin="-128.9031" TopMargin="49.7797" BottomMargin="-49.7797" FontSize="25" LabelText="Facebook: 9891XS" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                            <Size X="227.0000" Y="30.0000" />
                            <AnchorPoint ScaleY="0.5000" />
                            <Position X="64.9031" Y="-34.7797" />
                            <Scale ScaleX="1.0000" ScaleY="1.0000" />
                            <CColor A="255" R="0" G="0" B="0" />
                            <PrePosition X="0.3982" Y="-1.1593" />
                            <PreSize X="1.3926" Y="1.0000" />
                            <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                            <OutlineColor A="255" R="255" G="0" B="0" />
                            <ShadowColor A="255" R="110" G="110" B="110" />
                          </AbstractNodeData>
                        </Children>
                        <AnchorPoint ScaleY="0.5000" />
                        <Position X="33.1200" Y="72.4547" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.0460" Y="0.1650" />
                        <PreSize X="0.2264" Y="0.0683" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line" ActionTag="1164219313" Tag="1634" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.7200" RightMargin="0.7200" TopMargin="288.0000" BottomMargin="149.0000" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="150.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.5000" Y="0.3417" />
                        <PreSize X="0.9980" Y="0.0046" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="change_txt" ActionTag="264377782" Tag="1635" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="76.0000" RightMargin="76.0000" TopMargin="298.5072" BottomMargin="110.4928" FontSize="25" LabelText="Mật khẩu cho hình thức 1,2 và 3: vừa thay đổi" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="568.0000" Y="30.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="360.0000" Y="125.4928" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="0" G="0" B="0" />
                        <PrePosition X="0.5000" Y="0.2859" />
                        <PreSize X="0.7889" Y="0.0683" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                      <AbstractNodeData Name="line_0" ActionTag="-38473141" Tag="1636" IconVisible="False" PositionPercentXEnabled="True" PercentWidthEnable="True" PercentWidthEnabled="True" LeftMargin="0.0720" RightMargin="1.3680" TopMargin="339.4396" BottomMargin="97.5604" TouchEnable="True" ClipAble="False" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
                        <Size X="718.5600" Y="2.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="359.3520" Y="98.5604" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="255" G="255" B="255" />
                        <PrePosition X="0.4991" Y="0.2245" />
                        <PreSize X="0.9980" Y="0.0046" />
                        <SingleColor A="255" R="0" G="0" B="0" />
                        <FirstColor A="255" R="150" G="200" B="255" />
                        <EndColor A="255" R="255" G="255" B="255" />
                        <ColorVector ScaleY="1.0000" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="512.0000" Y="100.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5000" Y="0.5000" />
                    <PreSize X="0.7031" Y="2.1950" />
                    <FileData Type="Normal" Path="account/new_v4/box.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="384.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="0.2604" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </AbstractNodeData>
              <AbstractNodeData Name="next_button" ActionTag="258480769" CallBackType="Click" CallBackName="onNextButtonClicked" Tag="581" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="117.0000" RightMargin="117.0000" TopMargin="626.8193" BottomMargin="53.1807" TouchEnable="True" FontSize="30" ButtonText="Tiếp Tục" Scale9Enable="True" LeftEage="28" RightEage="28" TopEage="11" BottomEage="11" Scale9OriginX="28" Scale9OriginY="11" Scale9Width="3" Scale9Height="66" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="790.0000" Y="88.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="512.0000" Y="97.1807" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1265" />
                <PreSize X="0.7715" Y="0.1146" />
                <FontResource Type="Normal" Path="fonts/Montserrat-Bold.ttf" Plist="" />
                <TextColor A="255" R="255" G="255" B="255" />
                <DisabledFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v4/connection_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="btn_back" ActionTag="-1583506299" CallBackType="Click" CallBackName="onCancel" Tag="55" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.2000" RightMargin="923.8000" TopMargin="16.8176" BottomMargin="653.1824" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="68" Scale9Height="76" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="98.0000" Y="98.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="51.2000" Y="702.1824" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.0500" Y="0.9143" />
                <PreSize X="0.0957" Y="0.1276" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
                <NormalFileData Type="Normal" Path="account/new_v3/button_back_left.png" Plist="" />
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