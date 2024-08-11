<template>
	<view class="wrap">		
		<view class="dev-interface">
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">湿度</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{humi}}%</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">温度</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{temp}}℃</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">呼吸率</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{RespiratoryRate}}bpm</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">血氧</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{SaO2}}%</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">状态</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{Status}}</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">心率</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<view class="dev-data">{{heartHate}}bpm</view>
			</view>
			
			<view class="dev-card">
				<view class="">
					<view class="dev-txt">灯</view>
					<image class="dev-logo" src="../../static/logo.png" mode=""></image>
				</view>
				<switch :checked="LED" @change="onLedSeitch" color="#007aff" />	
			</view>
			
		</view>
	</view>
</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				humi: 0,
				temp: 0,
				LED: true,
				RespiratoryRate:0,
				SaO2:0,
				Status:0,
				heartHate:0,
				token: '',
			}
		},
		onLoad() {
			const params = {
				author_key: '/gw+gMmreFI/qg/BvWtBrPCu5SnkitpI8mvB9Fj6UDauSObtOhFq0rEMoFI04/pa9XR9N7GUsbpROEffXwYSsw==',
				version: '2022-05-01',
				user_id: '399443',
			}
			this.token = createCommonToken(params);
		},
		
		onShow(){
			this.fetchDevData();
			setInterval(()=>{
				this.fetchDevData();
			},2000)
		},
		
		methods: {
			fetchDevData(){
				uni.request({
				    url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', //仅为示例，并非真实接口地址。
				    method:'GET',
					data: {
				        
						product_id: 'sjdcGGYyKs',
						device_name: 'd1'
				    },
				    header: {
				        'authorization': this.token //自定义请求头信息
				    },
				    success: (res) => {
				        console.log(res.data);
				        console.log(res.data.data[0].value);
				        this.LED=res.data.data[0].value==='true'?true:false;
				        
						console.log(res.data.data[1].value);
						this.RespiratoryRate=res.data.data[1].value;
							
						console.log(res.data.data[2].value);
						this.SaO2=res.data.data[2].value;
						
						console.log(res.data.data[3].description);
						this.Status=res.data.data[3].description;
						
						console.log(res.data.data[4].value);
						this.heartHate=res.data.data[4].value;
						
				        console.log(res.data.data[5].value);
				        this.humi=res.data.data[5].value;
				        
				        console.log(res.data.data[6].value);
				        this.temp=res.data.data[6].value;
				    }
				});
			},
			onLedSeitch(event){
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
				    url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
				    method:'POST',
					data: { 
						product_id: 'sjdcGGYyKs',
						device_name: 'd1',
						params: {"LED": value }
				    },
				    header: {
				        'authorization': this.token //自定义请求头信息
				    },
				    success: () => {
				        console.log('LED' + (value?' ON':' OFF')+'!');

				    }
				});
			}
		}
	}
</script>

<style>
	.wrap{
		padding: 32rpx;		//内边距
	}
	
	.dev-interface {
		display: flex;
		flex-wrap: wrap;
		justify-content: space-between;
		 
	}
	
	.dev-txt{
		font-size: 20rpx;		//字体大小
		color:#6d6d6d;			//字体颜色
		text-align: center;
		
	}
	
	.dev-data{
		font-size: 50rpx;		//字体大小
		color:#6d6d6d;			//字体颜色
		text-align: center;
	}
	
	.dev-card {
		height: 150rpx;
		width: 300rpx;
		border-radius: 30rpx;			//圆角
		box-shadow: 0 0 15rpx #ccc;		//阴影
		margin-top:30rpx;				//上边距
		display: flex;
		justify-content: space-around;
		align-items: center;			//居中
	}

	.dev-logo {
		height: 70rpx;
		width: 70rpx;
		margin-top: 10rpx;			//上边距
		
	}

</style>