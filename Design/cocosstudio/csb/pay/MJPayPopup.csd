<GameFile>
  <PropertyGroup Name="MJPayPopup" Type="Layer" ID="aca50b71-795f-4690-bf6b-13d3844ea600" Version="3.10.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CustomClassName="MJPayPopup" Tag="7" ctype="GameLayerObjectData">
        <Size X="1024.0000" Y="768.0000" />
        <Children>
          <AbstractNodeData Name="Panel_2" ActionTag="-219944786" Tag="28" IconVisible="False" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" TouchEnable="True" ClipAble="False" BackColorAlpha="204" ComboBoxIndex="1" ColorAngle="151.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="1024.0000" Y="768.0000" />
            <AnchorPoint />
            <Position />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition />
            <PreSize X="1.0000" Y="1.0000" />
            <SingleColor A="255" R="26" G="26" B="26" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleX="-0.8746" ScaleY="0.4848" />
          </AbstractNodeData>
          <AbstractNodeData Name="popup" ActionTag="-589916819" Tag="9" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="162.0000" RightMargin="162.0000" TopMargin="84.0000" BottomMargin="84.0000" TouchEnable="True" ClipAble="False" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" Scale9Width="1" Scale9Height="1" ctype="PanelObjectData">
            <Size X="700.0000" Y="600.0000" />
            <Children>
              <AbstractNodeData Name="Image_1" ActionTag="1177903880" Tag="8" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" PercentWidthEnable="True" PercentHeightEnable="True" PercentWidthEnabled="True" PercentHeightEnabled="True" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="10" Scale9Height="10" ctype="ImageViewObjectData">
                <Size X="700.0000" Y="600.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="300.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.5000" />
                <PreSize X="1.0000" Y="1.0000" />
                <FileData Type="Normal" Path="mjstory/pay/pay_popup_background.png" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_1" ActionTag="-546277105" CallBackType="Click" CallBackName="onClose" Tag="10" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="650.0000" RightMargin="-50.0000" TopMargin="-50.0000" BottomMargin="550.0000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="70" Scale9Height="78" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="100.0000" Y="100.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="700.0000" Y="600.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="1.0000" Y="1.0000" />
                <PreSize X="0.1429" Y="0.1667" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/pay/pay_popup_close_button.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Text_9" ActionTag="-366266619" Tag="27" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="19.5000" RightMargin="19.5000" TopMargin="90.5000" BottomMargin="450.5000" FontSize="48" LabelText="Chọn hình thức thanh toán" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="661.0000" Y="59.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="480.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.8000" />
                <PreSize X="0.9443" Y="0.0983" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lb_device" ActionTag="2047769245" Tag="66" IconVisible="False" PositionPercentXEnabled="True" VerticalEdge="BottomEdge" LeftMargin="75.0000" RightMargin="75.0000" TopMargin="492.5000" BottomMargin="72.5000" IsCustomSize="True" FontSize="20" LabelText="* Mỗi mã kích hoạt được phép sử dụng trên 2 thiết bị" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                <Size X="550.0000" Y="35.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="90.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="0" G="0" B="0" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.7857" Y="0.0583" />
                <FontResource Type="Normal" Path="fonts/monkeystories_medium.ttf" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2" ActionTag="1325746964" CallBackType="Click" CallBackName="onOnePay" Tag="1" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="297.5000" BottomMargin="237.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="5" Scale9Height="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="650.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0" ActionTag="-1059268111" Tag="14" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="108.5000" RightMargin="267.5000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="22" LabelText="Thanh toán qua OnePay" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="274.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="245.5000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3777" Y="0.5000" />
                    <PreSize X="0.4215" Y="0.4154" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-1131298316" Tag="12" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="15.0000" RightMargin="555.0000" TopMargin="12.5000" BottomMargin="12.5000" Scale9Enable="True" LeftEage="14" RightEage="14" TopEage="14" BottomEage="14" Scale9OriginX="14" Scale9OriginY="14" Scale9Width="2" Scale9Height="2" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="40.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="-1994226895" Tag="13" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="3.0000" RightMargin="3.0000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="20" LabelText="Cách 2" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="74.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="20.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9250" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="55.0000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0846" Y="0.5000" />
                    <PreSize X="0.1231" Y="0.6154" />
                    <FileData Type="Normal" Path="mjstory/new_ui/frame_collection_more.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="270.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.4500" />
                <PreSize X="0.9286" Y="0.1083" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2_0" ActionTag="-416010908" CallBackType="Click" CallBackName="onCOD" Tag="2" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="207.5000" BottomMargin="327.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="5" Scale9Height="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="650.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0" ActionTag="520410098" Tag="16" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="103.5000" RightMargin="368.5000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="22" LabelText="Thu tiền tại nhà" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="178.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="192.5000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.2962" Y="0.5000" />
                    <PreSize X="0.2738" Y="0.4154" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-1876212336" Tag="17" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="15.0000" RightMargin="555.0000" TopMargin="12.5000" BottomMargin="12.5000" Scale9Enable="True" LeftEage="14" RightEage="14" TopEage="14" BottomEage="14" Scale9OriginX="14" Scale9OriginY="14" Scale9Width="2" Scale9Height="2" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="40.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="1720364801" Tag="18" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.5000" RightMargin="2.5000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="20" LabelText="Cách 1" HorizontalAlignmentType="HT_Center" VerticalAlignmentType="VT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="75.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="20.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9375" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="55.0000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0846" Y="0.5000" />
                    <PreSize X="0.1231" Y="0.6154" />
                    <FileData Type="Normal" Path="mjstory/new_ui/frame_collection_more.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="360.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.6000" />
                <PreSize X="0.9286" Y="0.1083" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2_1" ActionTag="-566962886" CallBackType="Click" CallBackName="onBankTransfer" Tag="3" IconVisible="False" PositionPercentXEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="387.5000" BottomMargin="147.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="5" Scale9Height="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="650.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0" ActionTag="191006599" Tag="20" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="105.0000" RightMargin="250.0000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="22" LabelText="Chuyển khoản ngân hàng" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="295.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="252.5000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.3885" Y="0.5000" />
                    <PreSize X="0.4538" Y="0.4154" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="-1000937860" Tag="21" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="15.0000" RightMargin="555.0000" TopMargin="12.5000" BottomMargin="12.5000" Scale9Enable="True" LeftEage="14" RightEage="14" TopEage="14" BottomEage="14" Scale9OriginX="14" Scale9OriginY="14" Scale9Width="2" Scale9Height="2" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="40.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="-1116481920" Tag="22" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.5000" RightMargin="2.5000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="20" LabelText="Cách 3" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="75.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="20.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9375" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="55.0000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0846" Y="0.5000" />
                    <PreSize X="0.1231" Y="0.6154" />
                    <FileData Type="Normal" Path="mjstory/new_ui/frame_collection_more.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="180.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.3000" />
                <PreSize X="0.9286" Y="0.1083" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="Button_2_2" ActionTag="-472972794" CallBackType="Click" CallBackName="onStore" Tag="4" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="25.0000" RightMargin="25.0000" TopMargin="477.5000" BottomMargin="57.5000" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="20" RightEage="20" TopEage="20" BottomEage="20" Scale9OriginX="20" Scale9OriginY="20" Scale9Width="5" Scale9Height="5" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="650.0000" Y="65.0000" />
                <Children>
                  <AbstractNodeData Name="Text_1_0" ActionTag="37684455" Tag="24" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="102.5000" RightMargin="56.5000" TopMargin="19.0000" BottomMargin="19.0000" FontSize="22" LabelText="Qua thẻ tín dụng đăng ký với Apple/Google" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                    <Size X="491.0000" Y="27.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="348.0000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5354" Y="0.5000" />
                    <PreSize X="0.7554" Y="0.4154" />
                    <FontResource Type="Normal" Path="fonts/Montserrat-SemiBold.ttf" Plist="" />
                    <OutlineColor A="255" R="255" G="0" B="0" />
                    <ShadowColor A="255" R="110" G="110" B="110" />
                  </AbstractNodeData>
                  <AbstractNodeData Name="Image_2" ActionTag="1633089537" Tag="25" IconVisible="False" PositionPercentYEnabled="True" HorizontalEdge="LeftEdge" LeftMargin="15.0000" RightMargin="555.0000" TopMargin="12.5000" BottomMargin="12.5000" Scale9Enable="True" LeftEage="14" RightEage="14" TopEage="14" BottomEage="14" Scale9OriginX="14" Scale9OriginY="14" Scale9Width="2" Scale9Height="2" ctype="ImageViewObjectData">
                    <Size X="80.0000" Y="40.0000" />
                    <Children>
                      <AbstractNodeData Name="Text_1" ActionTag="-1838908336" Tag="26" IconVisible="False" PositionPercentXEnabled="True" PositionPercentYEnabled="True" LeftMargin="2.5000" RightMargin="2.5000" TopMargin="8.0000" BottomMargin="8.0000" IsCustomSize="True" FontSize="20" LabelText="Cách 4" HorizontalAlignmentType="HT_Center" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="TextObjectData">
                        <Size X="75.0000" Y="24.0000" />
                        <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                        <Position X="40.0000" Y="20.0000" />
                        <Scale ScaleX="1.0000" ScaleY="1.0000" />
                        <CColor A="255" R="26" G="26" B="26" />
                        <PrePosition X="0.5000" Y="0.5000" />
                        <PreSize X="0.9375" Y="0.6000" />
                        <FontResource Type="Normal" Path="fonts/Montserrat-Medium.ttf" Plist="" />
                        <OutlineColor A="255" R="255" G="0" B="0" />
                        <ShadowColor A="255" R="110" G="110" B="110" />
                      </AbstractNodeData>
                    </Children>
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="55.0000" Y="32.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.0846" Y="0.5000" />
                    <PreSize X="0.1231" Y="0.6154" />
                    <FileData Type="Normal" Path="mjstory/new_ui/frame_collection_more.png" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="350.0000" Y="90.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5000" Y="0.1500" />
                <PreSize X="0.9286" Y="0.1083" />
                <TextColor A="255" R="65" G="65" B="70" />
                <NormalFileData Type="Normal" Path="mjstory/new_ui/button_intro_grade_blue.png" Plist="" />
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